#define	WM_ICON_NOTIFY			WM_USER+10       // 사용자 정의 메세지
#define WM_BARCODE_MSG			WM_USER + 10615
#define WM_RFID_MSG				WM_USER + 31121

#define WM_CLIENT_MSG			WM_USER + 500
#define WM_CLIENT_MSG_1			WM_USER + 500
#define WM_CLIENT_MSG_2			WM_USER + 501
#define WM_CLIENT_MSG_3			WM_USER + 502
#define WM_CLIENT_MSG_4			WM_USER + 503
#define WM_CLIENT_MSG_5			WM_USER + 504
#define WM_CLIENT_MSG_6			WM_USER + 505
#define WM_CLIENT_MSG_7			WM_USER + 506
#define WM_CLIENT_MSG_8			WM_USER + 507
#define WM_CLIENT_MSG_9			WM_USER + 508
#define WM_CLIENT_MSG_10		WM_USER + 509

#define WM_SERVER_MSG			WM_USER + 550
#define WM_SERVER_MSG_1			WM_USER + 550
#define WM_SERVER_MSG_2			WM_USER + 551
#define WM_SERVER_MSG_3			WM_USER + 552
#define WM_SERVER_MSG_4			WM_USER + 553
#define WM_SERVER_MSG_5			WM_USER + 554
#define WM_SERVER_MSG_6			WM_USER + 555
#define WM_SERVER_MSG_7			WM_USER + 556
#define WM_SERVER_MSG_8			WM_USER + 557
#define WM_SERVER_MSG_9			WM_USER + 558
#define WM_SERVER_MSG_10		WM_USER + 559

#define BARCODE_DATA			100
#define BARCODE_ERROR			101
#define BARCODE_TRIGGER_1		102
#define BARCODE_TRIGGER_2		103
#define BARCODE_DELETE			120

#define BARCODE_DATA_1			100
#define BARCODE_ERROR_1			101
#define BARCODE_TRIGGER_1_1		102
#define BARCODE_TRIGGER_1_2		103

#define BARCODE_DATA_2			200
#define BARCODE_ERROR_2			201
#define BARCODE_TRIGGER_2_1		202
#define BARCODE_TRIGGER_2_2		203

#define BARCODE_DATA_3			300
#define BARCODE_ERROR_3			301
#define BARCODE_TRIGGER_3_1		302
#define BARCODE_TRIGGER_3_2		303

#define BARCODE_DATA_4			400
#define BARCODE_ERROR_4			401
#define BARCODE_TRIGGER_4_1		402
#define BARCODE_TRIGGER_4_2		403

#define BARCODE_DATA_5			500
#define BARCODE_ERROR_5			501
#define BARCODE_TRIGGER_5_1		502
#define BARCODE_TRIGGER_5_2		503

#define RFID_DATA				100
#define RFID_ERROR				101
#define RFID_READ				102
#define RFID_WRITE				103
#define RFID_WRITE_GOOD			104
#define RFID_WRITE_NG			105
#define RFID_READ_GOOD			106
#define RFID_READ_NG			107
#define RFID_DELETE				120

#define RET_ERROR				0
#define RET_GOOD				1

#define CLIENT_CONNECT			0
#define CLIENT_CLOSE			1
#define CLIENT_SEND				2
#define CLIENT_REV				3
#define CLIENT_ACCEPT			4

#define SERVER_CONNECT			0
#define SERVER_CLOSE			1
#define SERVER_SEND				2
#define SERVER_REV				3
#define SERVER_ACCEPT			4

// 칼라 정의
#define SKY_C					RGB(187, 250, 255)
#define NOR_C					RGB(164, 164, 164)
#define NOR_L					RGB(190, 190, 190)
#define RED_C					RGB(255, 0, 0)
#define RED_L					RGB(255, 150, 150)
#define RED_D					RGB(80, 20, 0)
#define BLUE_C					RGB(0, 0, 255)
#define WHITE_C					RGB(255, 255, 255)
#define BLACK_C					RGB(25, 1, 1)
#define BLACK_L					RGB(62, 55, 55)
#define GRAY					RGB(90, 80, 80)
#define YELLOW_C				RGB(255,255,0)
#define YELLOW_D				RGB(128, 128, 0)
#define YELLOW_L				RGB(255, 255, 193)
#define GREEN_C					RGB(0,255,0)
#define GREEN_L					RGB(150,255,150)
#define GREEN_D					RGB(0,100,20)
#define DSERBLE_BC				RGB(210,210,230)
#define TEXT_BC					RGB(220,230,210)
#define OK_BC					RGB(0, 0, 255)
#define NG_BC					RGB(255, 0, 0)
#define RESULT_BC				RGB(40, 1, 1)
#define ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define BLACK					RGB(0, 0, 0)
#define BLUE_D					RGB(0, 0, 128)
#define BLUE				    RGB(0, 0, 255)
#define PINK					RGB(255, 125, 225)
#define TEXT_BC_1				RGB(240, 240, 240210)
#define BLUE_L					RGB(124, 139, 252)
#define ON_C					RGB(0, 255, 0)
#define OFF_C					RGB(0, 150, 0)
#define NG_C					0x0000FF
#define OK_C					0xFF00
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define TEST_BC					RGB(194, 231, 150)
#define IN_BC					RGB(204, 153, 255)

#define DVC_READY_C				RGB(153, 153, 255)
#define DVC_LOAD_C				RGB(239, 214, 198)
#define DVC_NOVIDEO_C			RGB(0,100,20)
#define DVC_PGM_C				RGB(255, 255, 0)
#define DVC_UNLOAD_C			RGB(255, 255, 204)
#define DVC_NONE_C				RGB(150, 150, 150)
#define DVC_START_C				RGB(150, 255, 150)
#define DVC_NO_C				RGB(128, 128, 0)

#define THICKNESS_FAIL			RGB(255,255,0)
#define LENGTH_FAIL				RGB(0xFF, 0x68, 0x20)
#define BARCODE_FAIL			RGB(255, 0, 0)

#define WINDOW_DN				RGB(180, 180, 180)
#define WINDOW_UP				RGB(250, 250, 250)

#define WINDOW_DN1				RGB(219, 255, 210)
#define WINDOW_UP1				RGB(250, 250, 250)

#define BLUE_L					RGB(124, 139, 252)
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define PINK_D					RGB(252, 233, 218)
#define COL_PUPLE				RGB(204,102,204)

// board 상태 정보
#define BIN_OFF					RGB(164, 164, 164)
#define BIN_NONE				RGB(62,	55, 55)
#define BIN_LOAD				RGB(187, 250, 255)
#define BIN_READY				RGB(150, 255, 150)
#define BIN_START				RGB(255, 255, 0)
#define BIN_ABORT				RGB(204, 102, 204)
#define BIN_GOOD				RGB(124, 139, 252)
#define BIN_FAIL				RGB(255, 0, 0)
#define BIN_RELOAD				RGB(255, 150, 150)

#define COM_PORT				0
#define COM_BAUDRATE			1
#define COM_DATA				2
#define COM_STOP				3
#define COM_PARITY				4

#define NO						0
#define YES						1

#define BIN_NOTUESD				0
#define BIN_LDBUFFERBIN			1
#define BIN_RETESTBIN			2
#define BIN_GOODBIN				3
#define BIN_REJECTBIN			4
#define BIN_REJECTBIN_1			5
#define BIN_REJECTBIN_2			6
#define BIN_REJECTBIN_3			7
#define BIN_REJECTBIN_4			8

struct tagSERIAL_INFO
{
	int nSerialPort;
	int nSerialBaudrate;
	int nSerialData;
	int nSerialParity;
	int nSerialStop;

	CString strOn;

	int nDataSub;

	BYTE byEnd[2];
};
extern  tagSERIAL_INFO  st_serial_info[5];

struct tagCLIENT_INFO
{
	CString		strIp;			// ip address......
	
	int			nPort;			// port.....
	int			nConnect;		// client 연결상태......
	int			nSendLength;	// 송신 문자 길이......
	int			nRevLength;		// 수신 문자 길이......
	int			nClientReady;	// 데이터 전송중...
	int			nRevInfo;		// client 종료 방법..... (0:바로 1: 수신후)

	char		chRev[8196];	// 수신내용....
	CString     strRev;
	char		chSend[8196];   // 송신내용....
	CString		strSend;
};
extern tagCLIENT_INFO st_client_info;

struct tagRFID_INFO
{
	bool		bChgFlag;	//Data가 변경되었는지 기록하는 Flag.
	int			nCh;		//Rfid Channel 변수
	int			nTagNum;	//Rfid Tag 갯수
	int			nRfidPos[50];
	int			nRfidNum[50];
	int			nRfidTag[50];

	CString		strRfidData[2][50];
	CString		strRfidTitle[50];
	CString		strRfidTag;
};
extern tagRFID_INFO st_rfid_info;