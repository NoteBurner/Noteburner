
typedef const struct _s_FuncInfo
{
	unsigned int		magicNumber;		// Identifies version of compiler
	int		            maxState;			// Highest state number plus one (thus
											// number of entries in unwind map)
	int					dispUnwindMap;		// Image relative offset of the unwind map
	unsigned int		nTryBlocks;			// Number of 'try' blocks in this function
	int					dispTryBlockMap;	// Image relative offset of the handler map
	unsigned int		nIPMapEntries;		// # entries in the IP-to-state map. NYI (reserved)
	int					dispIPtoStateMap;	// Image relative offset of the IP to state map
	int					dispUwindHelp;		// Displacement of unwind helpers from base
	int					dispESTypeList;		// Image relative list of types for exception specifications
	int					EHFlags;			// Flags for some features.
} FuncInfo;

typedef const struct _s_UnwindMapEntry {
	int	        toState;					// State this action takes us to
	int			action;						// Image relative offset of funclet
} UnwindMapEntry;


typedef const struct _s_TryBlockMapEntry {
	int		tryLow;				// Lowest state index of try
	int		tryHigh;			// Highest state index of try
	int		catchHigh;			// Highest state index of any associated catch
	int		nCatches;			// Number of entries in array
	int		dispHandlerArray;	// Image relative offset of list of handlers for this try
} TryBlockMapEntry;


typedef struct IptoStateMapEntry {
	unsigned int	Ip;		// Image relative offset of IP
	int	            State;
} IptoStateMapEntry;



typedef const struct _s_HandlerType {
	unsigned int	adjectives;			// Handler Type adjectives (bitfield)
	int				dispType;			// Image relative offset of the corresponding type descriptor
	int				dispCatchObj;		// Displacement of catch object from base
	int				dispOfHandler;		// Image relative offset of 'catch' code
	int				dispFrame;			// displacement of address of function frame wrt establisher frame
} HandlerType;


typedef struct TypeDescriptor
{
	const void * pVFTable;	// Field overloaded by RTTI
	void *	spare;			// reserved, possible for RTTI
	char	name[];			// The decorated name of the type; 0 terminated.
} TypeDescriptor;


typedef const struct _s_ESTypeList
{
	int 			nCount;			// how many types are there
	int				dispTypeArray;	// offset of list of types in exception specification
}ESTypeList;


/*
FuncInfo        struc ; (sizeof=0x28, align=0x4)
	magicNumber      dd ?  ; ��ʶ�������İ汾
	maxState         dd ?  ; ���ջչ�������±�ֵ
	dispUnwindMap    dd ?  ; ָ��ջչ���������ƫ������ָ��UnwindMapEntry�ṹ
	nTryBlocks       dd ?  ; try������
	dispTryBlockMap  dd ?  ; try���б��ƫ������ָ��TryBlockMapEntry�ṹ
	nIPMapEntries    dd ?  ; IP״̬ӳ��������
	dispIPtoStateMap dd ?  ; IP״̬ӳ����ƫ������ָ��IptoStateMapEntry�ṹ
	dispUwindHelp    dd ?  ; �쳣չ��������ƫ����
	dispESTypeList   dd ?  ; �쳣�����б��ƫ����
	EHFlags          dd ?  ; һЩ���ܵı�־
FuncInfo        ends

UnwindMapEntry  struc ; (sizeof=0x8, align=0x4)
	toState         dd ?   ; ջչ�����±�ֵ
	action          dd ?   ; չ��ִ�к���
UnwindMapEntry  ends

TryBlockMapEntry struc ; (sizeof=0x14, align=0x4)
	tryLow           dd ?   ; try�����С״̬���������ڷ�Χ���
	tryHigh          dd ?   ; try������״̬���������ڷ�Χ���
	catchHigh        dd ?   ; catch������״̬���������ڷ�Χ���
	nCatches         dd ?   ; catch����� 
	dispHandlerArray dd ?   ; catch�����������ƫ������ָ��HandlerType�ṹ 
TryBlockMapEntry ends

HandlerType     struc ; (sizeof=0x14, align=0x4)
	adjectives      dd ?    ; ����catch���ƥ����
	dispType        dd ?	; catch��Ҫ��׽�����͵�ƫ���� ��ָ��C++ RTTI���������ṹtype_info��ָ��
	dispCatchObj    dd ?	; ���ڶ�λ�쳣�����ƫ����
	dispOfHandler   dd ?	; catch������ƫ����
	dispFrame       dd ?	; �쳣�����Ϣ��ƫ����
HandlerType     ends


IptoStateMapEntry struc ; (sizeof=0x8, align=0x4)
	_Ip             dd ?	; try�����ʼRIP��ƫ����
	State           dd ?	; try״̬����
IptoStateMapEntry ends


ESTypeList      struc ; (sizeof=0x8, align=0x4)
	nCount          dd ?	; ������������
	dispTypeArray   dd ?	; ���������ƫ����
ESTypeList      ends
*/