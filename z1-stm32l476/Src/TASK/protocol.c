
const char m_BootVer[] = BOOT_VAR;

static UINT Boot_Read(UCHAR ucFlashType,UINT uiAddr,UCHAR *pucData,UINT uiDataLen);
int boot_download(unsigned char command, unsigned char *buf, unsigned int len);
void boot_set_err(void);

unsigned int PowerKeyCheck(void);
void PowerSystemPowerOff(void);

#define SYMBOL_STX 0x02
#define SYMBOL_ETX 0x03
#define COMM_RECV_BUF 1500
#define COMM_SEND_BUF 64
#define COMM_MAX_ARRAY 32

typedef enum
{
    COMM_ERROR_SUCCESS	= 0x00,
    COMM_ERROR_MAC		= 0xED,
    COMM_ERROR_TIMEOUT	= 0xEF,
    COMM_ERROR_HARDWARE	= 0xFC,
    COMM_ERROR_DATA		= 0xFD,
    COMM_ERROR_COMM_TYPE= 0xFE,
    COMM_ERROR_UNKNOWN	= 0xFF
} COMM_ERROR_TYPE;

typedef struct {
    uint16_t Tag;
    uint16_t Len;
    uint8_t* Val;
} TLVItem_t;

// 接收状态机
enum CommRecvSTM_e {
    waitingSTX,
    waitingSerialH,
    waitingSerialL,
    waitingLenH,
    waitingLenL,
    waitingData,
    waitingETX,
    waitingLRC,
    processing
};

static TLVItem_t m_InputTLVArray[COMM_MAX_ARRAY];
static int m_InputTLVArrayLength;

static TLVItem_t m_OutputTLVArray[COMM_MAX_ARRAY];
static int m_OutputTLVArrayLength;

volatile static enum CommRecvSTM_e m_CommRecvSTM;

static uint8_t m_RecvBuf[COMM_RECV_BUF]= {0};
volatile static int16_t m_RecvBufLen=0;

static uint8_t m_SendBuf[COMM_SEND_BUF]= {0};
volatile static int16_t m_SendBufLen=0;

volatile static uint16_t m_CommSerialNo=0;
volatile static uint16_t m_CommRecvPackDataLen=0;
volatile static int16_t m_CommRecvPendingPackDataLen=0;

const uint8_t constRetCMDType=0xFF;

const uint8_t constRetCMDNo0xDE=0xde;
const uint8_t constRetCMDNo0xDF=0xdf;
const uint8_t constRetCMDNo0xE0=0xe0;
const uint8_t constRetCMDNo0xE1=0xe1;

volatile static uint8_t m_KeyPreesedValue=0;
volatile static uint8_t m_bKeyPressed=0;
#define COMM_WAITKEY_RET_TIMEOUT -1
#define COMM_WAITKEY_RET_SERVERCANCEL -2
#define COMM_WAITUART_RET_TIMEOUT -3
#define COMM_WAITUART_RET_CANCEL -4



enum IAP_STM_e {
    wait_iaping,
    iaping,
    iap_over
};

enum IAP_CHANNEL_e {
    no_channel,
    usb_channel,
    ble_channel,
    com_channel
};
volatile static enum IAP_STM_e m_IAP_STM;
volatile static unsigned int m_iap_sector_addr;
volatile static unsigned char IAPBuf[SECTOR_SIZE + 128];
volatile static unsigned int m_iap_buflen;
volatile UINT   block_len;
volatile static unsigned int m_iap_err;
volatile static int m_first_1KB_data;
volatile static int m_iap_type;
#define CHACK_FLASH								m_iap_type

//volatile static unsigned int m_iap_sector;
//volatile static unsigned char m_iap_buf[SECTOR_SIZE + 128];
//volatile static unsigned int m_iap_bufcount;

volatile static enum IAP_CHANNEL_e m_iap_channel;

#define DOWN_DEBUG myPrintf




/********************************************************************************************************
** Function:通信协议
********************************************************************************************************/

static unsigned char LRC(unsigned char *auchMsg, unsigned short usDataLen)
{
    unsigned char uchLRC = 0 ; /* LRC char initialized */
    while (usDataLen--)    /* pass through message buffer */
        uchLRC^= *auchMsg++ ; /* use xor,do not use checksum */
//		uchLRC^= *auchMsg++ ; /* add buffer byte without carry */
    return uchLRC;
//	return ((unsigned char)(-((char)uchLRC))) ; /* return twos complement */
}


static void Comm_InitOutputTLVArray(void)
{
    m_OutputTLVArrayLength=0;
}

static int Comm_AppendToOutputTLVArray(uint16_t uiTag,uint16_t uiLen,uint8_t* pVal)
{
    if(m_OutputTLVArrayLength<COMM_MAX_ARRAY)
    {
        if(uiLen > 0)
        {
            m_OutputTLVArray[m_OutputTLVArrayLength].Tag=uiTag;
            m_OutputTLVArray[m_OutputTLVArrayLength].Len=uiLen;
            m_OutputTLVArray[m_OutputTLVArrayLength].Val=pVal;
            m_OutputTLVArrayLength ++;
            return 1;
        }
    }
    return 0;
}



void Comm_FunPackError(void * pContext)
{   //return code: FD
    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0002,0x0002,"FD");
}

void Comm_FunInvalidCMD(void * pContext)
{   //return code: FE
    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0002,0x0002,"FE");
}

// 拆开接收到的TLV
void Comm_unTLV(uint8_t* pStr,uint16_t TotalLen)
{
    uint16_t Tag;
    uint16_t Len;
    uint16_t ptr=0;

    // format: TLVTLVTLVTLV......
    m_InputTLVArrayLength = 0;

    while(ptr<TotalLen)
    {
        Tag = pStr[ptr];
        Tag = Tag<<8;
        ptr++;
        if(ptr>=TotalLen)
            break;
        Tag |= pStr[ptr];
        ptr++;
        if(ptr>=TotalLen)
            break;

        Len = pStr[ptr];
        Len = Len<<8;
        ptr++;
        if(ptr>=TotalLen)
            break;
        Len |= pStr[ptr];
        ptr++;
        if(ptr>=TotalLen)
            break;


        m_InputTLVArray[m_InputTLVArrayLength].Val = &(pStr[ptr]);
        m_InputTLVArray[m_InputTLVArrayLength].Tag = Tag;
        m_InputTLVArray[m_InputTLVArrayLength].Len = Len;
        m_InputTLVArrayLength++;
        ptr+= Len;

        if(ptr>=TotalLen)
            break;
    }

}

// 打包TLV数据
int Comm_encodeTLV(uint8_t* pBuf)
{
    int i;
    uint16_t j;
    uint16_t ptr=0;
    for(i=0; i<m_OutputTLVArrayLength; i++)
    {
        pBuf[ptr] = (m_OutputTLVArray[i].Tag >> 8) & 0xff;
        ptr++;
        pBuf[ptr] = (m_OutputTLVArray[i].Tag) & 0xff;
        ptr++;

        pBuf[ptr] = (m_OutputTLVArray[i].Len >> 8) & 0xff;
        ptr++;
        pBuf[ptr] = (m_OutputTLVArray[i].Len) & 0xff;
        ptr++;

        for(j=0; j<m_OutputTLVArray[i].Len; j++)
        {
            pBuf[ptr] = m_OutputTLVArray[i].Val[j];
            ptr++;
        }
    }

    return ptr;
}

int Comm_GetRecvCmd(void)
{
    int i;
    for(i=0; i<m_InputTLVArrayLength; i++)
    {
        if(m_InputTLVArray[i].Tag == 0x0001)
        {
            return m_InputTLVArray[i].Val[0];
        }
    }
    return -1;
}


void Comm_SendPackage(void)
{
    int i;

#ifdef __DEBUG
    if(uartNum != DEBUG_PRINTF_USE_UART_COM)//== BT_UART_COM)
    {
        MYDEBUG("UART%d Send:\r\n", uartNum);
        for(i=0; i<m_SendBufLen; i++)
        {
            MYDEBUG("%02X ",m_SendBuf[i]);
        }
        MYDEBUG("\r\n");
    }
#endif

    for(i=0; i<m_SendBufLen; i++)
    {
        OSUART_TxChar(COM_BOOT,m_SendBuf[i]);
    }
}

void Comm_FunToProcessCMD0xDE(void * pContext)
{
    //取版本号
    int i, ret;
    unsigned char command = 0;
    unsigned int len = 0;
    unsigned char *ptr = 0;

    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0000,0x0001,(uint8_t*)&(constRetCMDType));
    Comm_AppendToOutputTLVArray(0x0001,0x0001,(uint8_t*)&(constRetCMDNo0xDE));
    Comm_AppendToOutputTLVArray(0x0002,0x0002,"00");
    Comm_AppendToOutputTLVArray(0xBF31,strlen((char *)(m_BootVer)),(uint8_t*)&m_BootVer[0]);
    if (g_sn.active == 1)
        Comm_AppendToOutputTLVArray(0xBF07,strlen((char *)(g_sn.data)),g_sn.data);
}

void Comm_FunToProcessCMD0xDF(void * pContext)
{
    //测试命令，不进行任何动作，返回一个“00”就行
    int i, ret;
    unsigned char command = 0;
    unsigned int len = 0;
    unsigned char *ptr = 0;

    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0000,0x0001,(uint8_t*)&(constRetCMDType));
    Comm_AppendToOutputTLVArray(0x0001,0x0001,(uint8_t*)&(constRetCMDNo0xDF));
    Comm_AppendToOutputTLVArray(0x0002,0x0002,"00");
}

void Comm_FunToProcessCMD0xE0(void * pContext)
{
    //在线升级 USB or UART
    int i, ret;
    unsigned char command = 0;
    unsigned int len = 0;
    unsigned char *ptr = 0;

    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0000,0x0001,(uint8_t*)&(constRetCMDType));
    Comm_AppendToOutputTLVArray(0x0001,0x0001,(uint8_t*)&(constRetCMDNo0xE0));

    for(i=0; i<m_InputTLVArrayLength; i++)
    {
        if(m_InputTLVArray[i].Tag == 0x0002)
        {
            //命令
            command = *(m_InputTLVArray[i].Val);
        }
        else if(m_InputTLVArray[i].Tag == 0x0003)
        {
            //数据
            ptr = m_InputTLVArray[i].Val;
            len = m_InputTLVArray[i].Len;
        }
    }

    ret = boot_download(command, ptr, len);
    if (ret == 0)
        Comm_AppendToOutputTLVArray(0x0002,0x0002,"00");
    else if (ret == -2)
    {
        boot_set_err();
        Comm_AppendToOutputTLVArray(0x0002,0x0002,"FE");
    }
    else
    {
        boot_set_err();
        Comm_AppendToOutputTLVArray(0x0002,0x0002,"FF");
    }
}


void Comm_FunToProcessCMD0xE1(void * pContext)
{
    //
    int i, ret;
    unsigned char type = 0;


    for(i=0; i<m_InputTLVArrayLength; i++)
    {
        if(m_InputTLVArray[i].Tag == 0xBF32)
        {
            type = *m_InputTLVArray[i].Val;
        }
    }
    Comm_InitOutputTLVArray();
    Comm_AppendToOutputTLVArray(0x0000,0x0001,(uint8_t*)&(constRetCMDType));
    Comm_AppendToOutputTLVArray(0x0001,0x0001,(uint8_t*)&(constRetCMDNo0xE1));
    if ((type == 0) || (type == 1))
    {
        Comm_AppendToOutputTLVArray(0x0002,0x0002,"00");
    }
    else
    {
        Comm_AppendToOutputTLVArray(0x0002,0x0002,"FE");
    }
}

void Comm_OnProcess(void)
{
    uint16_t retTLVAreaLen=0;
    uint16_t retDataAreaLen=0;
    int CommandNo=0;
    uint16_t uiHeadLen=0;
    uint16_t uiBodyLen=0;
    int ptr=0;
    uint8_t calcedLRC;

    // calc LRC
    calcedLRC = (uint8_t)LRC(m_RecvBuf,m_RecvBufLen-1);
    if(calcedLRC != m_RecvBuf[m_RecvBufLen-1])
    {   // LRC fail
        Comm_FunPackError(NULL);
    }
    else
    {
        // resolve HEAD
        ptr=5;
        uiHeadLen = m_RecvBuf[ptr];
        uiHeadLen = uiHeadLen<<8;
        ptr++;
        uiHeadLen |= m_RecvBuf[ptr];
        ptr++;
        ptr+= uiHeadLen;

        // resolve BODY
        uiBodyLen = m_RecvBuf[ptr];
        uiBodyLen = uiBodyLen<<8;
        ptr++;
        uiBodyLen |= m_RecvBuf[ptr];
        ptr++;

        // unTLV body
        Comm_unTLV(&(m_RecvBuf[ptr]),uiBodyLen);
        CommandNo = Comm_GetRecvCmd();
        // 跳转具体命令的处理函数（阻塞型）
        if(CommandNo==constRetCMDNo0xE0)
        {
            Comm_FunToProcessCMD0xE0(NULL);
        }
        else if(CommandNo==constRetCMDNo0xDF)
        {
            Comm_FunToProcessCMD0xDF(NULL);
        }
        else if(CommandNo==constRetCMDNo0xDE)
        {
            Comm_FunToProcessCMD0xDE(NULL);
        }
        else if(CommandNo==constRetCMDNo0xE1)
        {
            Comm_FunToProcessCMD0xE1(NULL);
        }
        else
        {
            Comm_FunInvalidCMD(NULL);
        }
    }

    // generate return package
    retTLVAreaLen=Comm_encodeTLV(m_SendBuf+14);
    m_SendBuf[0]=SYMBOL_STX;
    m_SendBuf[1]=(m_CommSerialNo >> 8) & 0xff;
    m_SendBuf[2]=m_CommSerialNo & 0xff;
    retDataAreaLen = retTLVAreaLen + 9;
    m_SendBuf[3]=(retDataAreaLen >> 8) & 0xff;
    m_SendBuf[4]=retDataAreaLen & 0xff;
    m_SendBuf[5]=0x00;
    m_SendBuf[6]=0x05;
    m_SendBuf[7]=0xFF;
    m_SendBuf[8]=0x00;
    m_SendBuf[9]=0x00;
    m_SendBuf[10]=0x01;
    m_SendBuf[11]=0x31;
    m_SendBuf[12]=(retTLVAreaLen >> 8) & 0xff;
    m_SendBuf[13]=retTLVAreaLen & 0xff;
    m_SendBuf[14+retTLVAreaLen]=SYMBOL_ETX;
    m_SendBuf[14+retTLVAreaLen+1]=LRC(m_SendBuf,14+retTLVAreaLen+1);
    m_SendBufLen = 16+retTLVAreaLen;

    // send package
    Comm_SendPackage();

    // restore recv STM, wait next package
    m_RecvBufLen = 0;
    m_CommRecvSTM = waitingSTX;
}


//
void Comm_Protocol(uint8_t rx_byte)
{
    int i;
    if (m_RecvBufLen < COMM_RECV_BUF)
    {
        switch (m_CommRecvSTM)
        {
        case waitingSTX:
            if (rx_byte == SYMBOL_STX)
            {
                m_RecvBuf[m_RecvBufLen] = rx_byte;
                m_RecvBufLen++;
                m_CommRecvSTM = waitingSerialH;

            }
            else
            {
                m_RecvBufLen=0;
            }
            break;
        case waitingSerialH:
            m_CommSerialNo = rx_byte << 8;
            m_RecvBuf[m_RecvBufLen] = rx_byte;
            m_RecvBufLen++;
            m_CommRecvSTM = waitingSerialL;

            break;
        case waitingSerialL:
            m_CommSerialNo |= rx_byte ;
            m_RecvBuf[m_RecvBufLen] = rx_byte;
            m_RecvBufLen++;
            m_CommRecvSTM = waitingLenH;

            break;
        case waitingLenH:
            m_CommRecvPackDataLen = rx_byte << 8;
            m_RecvBuf[m_RecvBufLen] = rx_byte;
            m_RecvBufLen++;
            m_CommRecvSTM = waitingLenL;
            break;
        case waitingLenL:
            m_CommRecvPackDataLen |= rx_byte ;
            if(m_CommRecvPackDataLen>=COMM_RECV_BUF-7)
            {
                //长度太大，不允许继续接收
                //接收状态机复位，清除之前的接收缓冲
                //如果要返回一个错误包的话，需要在状态机中增加一个状态
                m_RecvBufLen = 0;
                m_CommRecvSTM = waitingSTX;
            }
            else
            {
                m_RecvBuf[m_RecvBufLen] = rx_byte;
                m_RecvBufLen++;
                m_CommRecvSTM = waitingData;
                m_CommRecvPendingPackDataLen = (int16_t)m_CommRecvPackDataLen;
            }
            break;
        case waitingData:
            m_CommRecvPendingPackDataLen--;
            if(m_CommRecvPendingPackDataLen>=0)
            {
                m_RecvBuf[m_RecvBufLen] = rx_byte;
                m_RecvBufLen++;
                if(m_CommRecvPendingPackDataLen==0)
                {
                    m_CommRecvSTM = waitingETX;
                }
            }
            else
            {
                m_CommRecvSTM = waitingETX;
            }
            break;
        case waitingETX:
            m_RecvBuf[m_RecvBufLen] = rx_byte;
            m_RecvBufLen++;
            m_CommRecvSTM = waitingLRC;
            break;
        case waitingLRC:
            m_RecvBuf[m_RecvBufLen] = rx_byte;
            m_RecvBufLen++;

            m_CommRecvSTM = processing;

            Comm_OnProcess();
            break;
        case processing:
            break;

        default:
            m_RecvBufLen = 0;
            m_CommRecvSTM = waitingSTX;
            break;
        }
    }
    else
    {
        m_RecvBufLen = 0;
        m_CommRecvSTM = waitingSTX;
    }
}

void Comm_Init(void)
{
    m_RecvBufLen = 0;
    m_SendBufLen = 0;

    m_CommRecvSTM = waitingSTX;
    m_CommSerialNo=0;
    m_CommRecvPackDataLen=0;
    m_CommRecvPendingPackDataLen=0;

    m_InputTLVArrayLength=0;
    m_OutputTLVArrayLength=0;

}



