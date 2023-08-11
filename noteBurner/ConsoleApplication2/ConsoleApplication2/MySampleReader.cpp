#include <iostream>
#include <iomanip>
#include "MySampleReader.h"
#include "MySampleDecrypter.h"
using namespace std;
#include "widevinecdm.h"



char errorbuf[1024];

AVCodec* Encodec = NULL;  //�������1

AVCodecContext* Enc = NULL; //�����

void SaveH264(AVPacket* packet)
{
	FILE* fpSave;
	if ((fpSave = fopen("BB.h264", "wb")) == NULL) //h264������ļ���  
	{
		std::cout << "fopen  SaveH264 NO";
		return;
	}
	fwrite(packet->data, 1, packet->size, fpSave);//д���ݵ��ļ���  

	fclose(fpSave);
}
void saveAVFrameAsYUV(AVFrame* frame, const std::string& saveFileName) {
	// ��ʼ��FFmpeg��


	// ��ʼ�����ظ�ʽת��������
	AVCodecContext* codecContext = avcodec_alloc_context3(NULL);
	if (!codecContext) {
		fprintf(stderr, "Could not allocate codec context\n");
		return;
	}

	codecContext->width = frame->width;
	codecContext->height = frame->height;
	codecContext->pix_fmt = AV_PIX_FMT_YUV420P; // Ŀ�����ظ�ʽ

	struct SwsContext* sws_ctx = sws_getContext(
		frame->width, frame->height, (AVPixelFormat)frame->format,
		codecContext->width, codecContext->height, codecContext->pix_fmt,
		SWS_BILINEAR, NULL, NULL, NULL
	);

	// ����һ��AVFrame���ڴ洢ת�����ͼ������
	AVFrame* convertedFrame = av_frame_alloc();
	convertedFrame->width = codecContext->width;
	convertedFrame->height = codecContext->height;
	convertedFrame->format = codecContext->pix_fmt;
	av_frame_get_buffer(convertedFrame, 32); // ����ͼ�������ڴ�

	// �������ظ�ʽת��
	sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height,
		convertedFrame->data, convertedFrame->linesize);

	// ����ת�����ͼ�����ݵ��ļ�
	FILE* outputFile = fopen(saveFileName.c_str(), "ab");
	if (!outputFile) {
		fprintf(stderr, "Could not open output file\n");
		av_frame_free(&convertedFrame);
		avcodec_free_context(&codecContext);
		sws_freeContext(sws_ctx);
		return;
	}

	for (int i = 0; i < codecContext->height; i++) {
		fwrite(convertedFrame->data[0] + i * convertedFrame->linesize[0], 1, codecContext->width, outputFile);
	}
	for (int i = 0; i < codecContext->height / 2; i++) {
		fwrite(convertedFrame->data[1] + i * convertedFrame->linesize[1], 1, codecContext->width / 2, outputFile);
		fwrite(convertedFrame->data[2] + i * convertedFrame->linesize[2], 1, codecContext->width / 2, outputFile);
	}

	fclose(outputFile);

	// ������Դ
	av_frame_free(&convertedFrame);
	avcodec_free_context(&codecContext);
	sws_freeContext(sws_ctx);
}
void saveFrameAsYUV420P10LE(const char* outputFilename, AVFrame* frame) {
	// ����� YUV �ļ�
	FILE* yuvFile = fopen(outputFilename, "ab");
	if (!yuvFile) {
		fprintf(stderr, "Could not open output YUV file\n");
		return;
	}

	// �� YUV420P10LE ����д������ļ�
	for (int i = 0; i < frame->height; i++) {
		fwrite(frame->data[0] + i * frame->linesize[0], 1, frame->width * 2, yuvFile);
	}
	for (int i = 0; i < frame->height / 2; i++) {
		fwrite(frame->data[1] + i * frame->linesize[1], 1, frame->width / 2 * 2, yuvFile);
		fwrite(frame->data[2] + i * frame->linesize[2], 1, frame->width / 2 * 2, yuvFile);
	}

	// �ر��ļ�
	fclose(yuvFile);
}


AP4_Result MySampleReader::ReadSampleData(AP4_Sample& sample, AP4_DataBuffer& sample_data)
{	

	auto printfHex = [](char *byteArray, int length) {
		
		// ��ӡʮ�������ַ���
		for (int i = 0; i < length; ++i) {
			std::cout << std::hex << std::setw(2) << std::setfill('0')
				<< static_cast<int>(byteArray[i]);
		}

		std::cout << std::endl;
	};
	int dataSize = sample.GetSize();
	char* data = new char[dataSize + 1];
	memset(data, 0, dataSize + 1);
	sample.GetDataStream()->Seek(sample.GetOffset());
	sample.GetDataStream()->Read(data, dataSize);
	

	
	AVFrame* frame;
	
	frame = av_frame_alloc();
	AVPacket* packet = av_packet_alloc();
	
	if (this->m_decrypter != nullptr) {

		
		AP4_Cardinal subsample_count =0;
		const AP4_UI16* bytes_of_cleartext_data;
		const AP4_UI32* bytes_of_encrypted_data;

		const AP4_UI08* iv = m_decrypter->m_table->GetIv(m_decrypter->index);
		uint32_t ivSize = m_decrypter->m_table->GetIvSize();
		int64_t timestamp = ((double)sample.GetDts() * ((double)1000000 / (double)m_decrypter->m_timeScale) + 0.5);


		m_decrypter->m_table->GetSampleInfo(m_decrypter->index, subsample_count, bytes_of_cleartext_data, bytes_of_encrypted_data);


		printf("read sample offset: 0x%llX size: 0x%X isEncrypted: true\n", sample.GetOffset(), sample.GetSize());
		printf("keyid: ");
		printfHex((char *)m_decrypter->m_key_id, m_decrypter->m_key_id_size);
		printf("iv: ");
		printfHex((char*)iv, ivSize);

		for (int i = 0; i < subsample_count; i++) {
			printf("subsample[%d] bytes_of_cleartext_data %X bytes_of_encrypted_data %X\n", i, bytes_of_cleartext_data[i], bytes_of_encrypted_data[i]);
		}
	
		m_decrypter->index++;
	}
	else {


		packet->data = (uint8_t*)data;
		packet->size = dataSize;
		packet->duration = sample.GetDuration();
		int error_code = av_packet_make_refcounted(packet);


		int sendCode = avcodec_send_packet(decodecContext, packet);
		if (sendCode >= 0) {
			while (1) {
				int arfcode = avcodec_receive_frame(decodecContext, frame);
				if (arfcode==0||arfcode == 0xDFB9B0BB || arfcode == AVERROR(EAGAIN)) {
					break;
				}

				if (arfcode < 0) {
					printf("receive frame failed \n");
					return 0;
				}

			}
		}
	
		
		char errbuf[64]{ 0 };
		//cout << "error_code : " << error_code << endl;
		printf("read sample offset: 0x%llX size: 0x%X isEncrypted: false\n", sample.GetOffset(), sample.GetSize());
		//if (sample.GetOffset() == 0xADBE9) {
		saveFrameAsYUV420P10LE( "corrent.yuv", frame);
		//	exit(0);
		//}
		
		static int p = 0;
		int ret = 1;
		
		frame->pts = p;

		p++;
		ret = avcodec_send_frame(encodecContext, frame);
		while (1) {
			
		
			AVPacket pkt{ 0 };
			av_init_packet(&pkt);
			// ��ȡ����������
			ret = avcodec_receive_packet(encodecContext, &pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				break;
			}
				
			else 
				if (ret < 0) {
				fprintf(stderr, "Error encoding a frame: %s\n", av_make_error_string(errbuf, sizeof(errbuf), ret));
				return 1007;
			}
			
			// ��pts���ŵ��������time_base��
			av_packet_rescale_ts(&pkt, encodecContext->time_base, videoStream->time_base);
			pkt.stream_index = videoStream->index;
			// ������д�뵽�����
			ret = av_interleaved_write_frame(outputFormatContext, &pkt);
			av_packet_unref(&pkt);
			
		}

	}

	av_packet_free(&packet);
	av_frame_free(&frame);
	delete data;
	
	return AP4_Result();
}
