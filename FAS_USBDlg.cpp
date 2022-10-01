// FAS_USBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FAS_USB.h"
#include "FAS_USBDlg.h"

#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmapButton* m_bbut;
__int16 regRGU;
bool bPusk;//признак нажат ли пуск

CFT_USB m_ft_usb;

FT_STATUS ft_st;

FILE* filelog;//лог-файл
char cReset[1];
BYTE IOD[129];
CString Rmax_in_device, Rmax_in_device1;

int RunTimer;
CString hint1 = "Начальное смещение цели по дальности относительно ОИ",
hint2 = "Конечный отсчет цели",
hint3 = "Смещение цели по азимуту относительно альфа0",
hint4 = "Размер цели по азимуту",
hint5 = "Величина скольжения",
hint6 = "Широкополосный режим",
hint7 = "Импульсно-кодовая модуляция",
hint8 = "ИКМ, модулированая сигналом цели",
hint9 = "Модулирование сигнала цели двумя частотами",
hint10 = "Переключение частот 56/66 MHz",
hint11 = Rmax_in_device;
/////////////////////////////////////////////////////////////////////////////
//Регистры ФАС
__int16 WR_RG_RK1 = 0x0197;
__int16 WR_RG_RN1 = 0x0297;
__int16 WR_RG_SHCOS = 0x0397;
__int16 WR_RG_M1 = 0x0497;
__int16 WR_RG_1N1 = 0x0597;
__int16 WR_RG_2N1 = 0x0697;
__int16 WR_RG_RGU = 0x0797;
__int16 WR_RG_RK2 = 0x0897;
__int16 WR_RG_RN2 = 0x0997;
__int16 WR_RG_dRN = 0x1097;//new
__int16 WR_RG_OI = 0x0A97;
__int16 WR_RG_M2 = 0x0B97;
__int16 WR_RG_1N2 = 0x0C97;
__int16 WR_RG_2N2 = 0x0D97;
__int16 WR_RG_RMAX = 0x0E98;
__int16 WR_RG_SHSIN = 0x0F97;
__int16 WR_RG_RK3 = 0x1197;
__int16 WR_RG_RN3 = 0x1297;
__int16 WR_RG_DN = 0x1397;
__int16 WR_RG_M3 = 0x1497;
__int16 WR_RG_1N3 = 0x1597;
__int16 WR_RG_2N3 = 0x1697;
__int16 WR_RG_ZDIKM = 0x1797;
__int16 WR_RG_RK4 = 0x1897;
__int16 WR_RG_RN4 = 0x1997;
__int16 WR_RG_dRk = 0x1A97;//new
__int16 WR_RG_M4 = 0x1B97;
__int16 WR_RG_1N4 = 0x1C97;
__int16 WR_RG_2N4 = 0x1D97;
__int16 WR_RG_d1N = 0x1E97;//new
__int16 WR_RG_d2N = 0x1F97;//new


char* pBuf_[5];
char buf1_[64] = " ";
char buf2_[64] = " ";
char buf3_[64] = " ";
char buf4_[64] = " ";
/////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
		//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBDlg dialog

CFAS_USBDlg::CFAS_USBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFAS_USBDlg::IDD, pParent)
{

	filelog = fopen("log.txt", "w+");


	//	LPBYTE lpb;
	pBuf_[0] = buf1_;
	pBuf_[1] = buf2_;
	pBuf_[2] = buf3_;
	pBuf_[3] = buf4_;
	pBuf_[4] = NULL;


	//{{AFX_DATA_INIT(CFAS_USBDlg)
	m_sRn1 = _T("1");
	m_sRk1 = _T("1");
	m_s1N1 = _T("0");
	m_s2N1 = _T("1");
	m_sF1 = _T("70");
	m_sRn2 = _T("2");
	m_sRk2 = _T("2");
	m_s1N2 = _T("0");
	m_s2N2 = _T("2");
	m_sF2 = _T("140");
	m_sRn3 = _T("3");
	m_sRk3 = _T("3");
	m_s1N3 = _T("0");
	m_s2N3 = _T("3");
	m_sF3 = _T("210");
	m_sRn4 = _T("4");
	m_sRk4 = _T("4");
	m_s1N4 = _T("0");
	m_s2N4 = _T("4");
	m_sF4 = _T("280");
	m_sRmax = _T("500");
	m_sdN = _T("");
	m_sA0 = _T("");
	m_sDF = _T("");
	m_sR = _T("0.5");
	m_sNOI = _T("8");
	m_sKOI = _T("9");
	m_sNumOI = _T("1000");
	m_sZD = _T("800");
	m_seddN = _T("255");
	m_sShSin = _T("0.5");
	m_sdN1 = _T("30");
	m_sdN2 = _T("40");
	m_sdN3 = _T("30");
	m_sdN4 = _T("40");
	m_sdR3 = _T("13");
	m_sdR4 = _T("14");
	m_sdR1 = _T("10");
	m_sdR2 = _T("20");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFAS_USBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFAS_USBDlg)
	DDX_Control(pDX, IDC_EDDR2, m_cdR2);
	DDX_Control(pDX, IDC_EDDR1, m_cdR1);
	DDX_Control(pDX, IDC_EDDN2, m_cdN2);
	DDX_Control(pDX, IDC_EDDN1, m_cdN1);
	DDX_Control(pDX, IDC_EDSHSIN, m_cShSin);
	DDX_Control(pDX, IDC_EDDN, m_ceddN);
	DDX_Control(pDX, IDC_EDZD, m_cZD);
	DDX_Control(pDX, IDC_EDNUMOI, m_cNumOI);
	DDX_Control(pDX, IDC_EDKOI, m_cKOI);
	DDX_Control(pDX, IDC_EDNOI, m_cNOI);
	DDX_Control(pDX, IDC_EDR, m_cR);
	DDX_Control(pDX, IDC_EDRMAX, m_cRmax);
	DDX_Control(pDX, IDC_CMBDF, m_cDF);
	DDX_Control(pDX, IDC_CMBA0, m_cA0);
	DDX_Control(pDX, IDC_CMBVSK, m_cdN);
	DDX_Control(pDX, IDC_EDF1, m_cF1);
	DDX_Control(pDX, IDC_ED2N1, m_c2N1);
	DDX_Control(pDX, IDC_ED1N1, m_c1N1);
	DDX_Control(pDX, IDC_EDRK1, m_cRk1);
	DDX_Control(pDX, IDC_EDRN1, m_cRn1);
	DDX_Text(pDX, IDC_EDRN1, m_sRn1);
	DDX_Text(pDX, IDC_EDRK1, m_sRk1);
	DDX_Text(pDX, IDC_ED1N1, m_s1N1);
	DDX_Text(pDX, IDC_ED2N1, m_s2N1);
	DDX_Text(pDX, IDC_EDF1, m_sF1);
	DDX_Control(pDX, IDC_EDF2, m_cF2);
	DDX_Control(pDX, IDC_ED2N2, m_c2N2);
	DDX_Control(pDX, IDC_ED1N2, m_c1N2);
	DDX_Control(pDX, IDC_EDRK2, m_cRk2);
	DDX_Control(pDX, IDC_EDRN2, m_cRn2);
	DDX_Text(pDX, IDC_EDRN2, m_sRn2);
	DDX_Text(pDX, IDC_EDRK2, m_sRk2);
	DDX_Text(pDX, IDC_ED1N2, m_s1N2);
	DDX_Text(pDX, IDC_ED2N2, m_s2N2);
	DDX_Text(pDX, IDC_EDF2, m_sF2);
	DDX_Control(pDX, IDC_EDF3, m_cF3);
	DDX_Control(pDX, IDC_ED2N3, m_c2N3);
	DDX_Control(pDX, IDC_ED1N3, m_c1N3);
	DDX_Control(pDX, IDC_EDRK3, m_cRk3);
	DDX_Control(pDX, IDC_EDRN3, m_cRn3);
	DDX_Text(pDX, IDC_EDRN3, m_sRn3);
	DDX_Text(pDX, IDC_EDRK3, m_sRk3);
	DDX_Text(pDX, IDC_ED1N3, m_s1N3);
	DDX_Text(pDX, IDC_ED2N3, m_s2N3);
	DDX_Text(pDX, IDC_EDF3, m_sF3);
	DDX_Control(pDX, IDC_EDF4, m_cF4);
	DDX_Control(pDX, IDC_ED2N4, m_c2N4);
	DDX_Control(pDX, IDC_ED1N4, m_c1N4);
	DDX_Control(pDX, IDC_EDRK4, m_cRk4);
	DDX_Control(pDX, IDC_EDRN4, m_cRn4);
	DDX_Text(pDX, IDC_EDRN4, m_sRn4);
	DDX_Text(pDX, IDC_EDRK4, m_sRk4);
	DDX_Text(pDX, IDC_ED1N4, m_s1N4);
	DDX_Text(pDX, IDC_ED2N4, m_s2N4);
	DDX_Text(pDX, IDC_EDF4, m_sF4);
	DDX_Text(pDX, IDC_EDRMAX, m_sRmax);
	DDX_CBString(pDX, IDC_CMBVSK, m_sdN);
	DDX_CBString(pDX, IDC_CMBA0, m_sA0);
	DDX_CBString(pDX, IDC_CMBDF, m_sDF);
	DDX_Text(pDX, IDC_EDR, m_sR);
	DDX_Text(pDX, IDC_EDNOI, m_sNOI);
	DDX_Text(pDX, IDC_EDKOI, m_sKOI);
	DDX_Text(pDX, IDC_EDNUMOI, m_sNumOI);
	DDX_Text(pDX, IDC_EDZD, m_sZD);
	DDX_Text(pDX, IDC_EDDN, m_seddN);
	DDX_Text(pDX, IDC_EDSHSIN, m_sShSin);
	DDX_Text(pDX, IDC_EDDN1, m_sdN1);
	DDX_Text(pDX, IDC_EDDN2, m_sdN2);
	DDX_Text(pDX, IDC_EDDR1, m_sdR1);
	DDX_Text(pDX, IDC_EDDR2, m_sdR2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFAS_USBDlg, CDialog)
	//{{AFX_MSG_MAP(CFAS_USBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_PUSK, OnPusk)
	ON_EN_KILLFOCUS(IDC_EDRN1, OnKillfocusEdrn1)
	ON_EN_KILLFOCUS(IDC_EDRK1, OnKillfocusEdrk1)
	ON_EN_KILLFOCUS(IDC_ED1N1, OnKillfocusEd1n1)
	ON_EN_KILLFOCUS(IDC_ED2N1, OnKillfocusEd2n1)
	ON_EN_KILLFOCUS(IDC_EDF1, OnKillfocusEdf1)
	ON_EN_KILLFOCUS(IDC_EDRN2, OnKillfocusEdrn2)
	ON_EN_KILLFOCUS(IDC_EDRK2, OnKillfocusEdrk2)
	ON_EN_KILLFOCUS(IDC_ED1N2, OnKillfocusEd1n2)
	ON_EN_KILLFOCUS(IDC_ED2N2, OnKillfocusEd2n2)
	ON_EN_KILLFOCUS(IDC_EDF2, OnKillfocusEdf2)
	ON_EN_KILLFOCUS(IDC_EDRN3, OnKillfocusEdrn3)
	ON_EN_KILLFOCUS(IDC_EDRK3, OnKillfocusEdrk3)
	ON_EN_KILLFOCUS(IDC_ED1N3, OnKillfocusEd1n3)
	ON_EN_KILLFOCUS(IDC_ED2N3, OnKillfocusEd2n3)
	ON_EN_KILLFOCUS(IDC_EDF3, OnKillfocusEdf3)
	ON_EN_KILLFOCUS(IDC_EDRN4, OnKillfocusEdrn4)
	ON_EN_KILLFOCUS(IDC_EDRK4, OnKillfocusEdrk4)
	ON_EN_KILLFOCUS(IDC_ED1N4, OnKillfocusEd1n4)
	ON_EN_KILLFOCUS(IDC_ED2N4, OnKillfocusEd2n4)
	ON_EN_KILLFOCUS(IDC_EDF4, OnKillfocusEdf4)
	ON_EN_KILLFOCUS(IDC_EDRMAX, OnKillfocusEdrmax)
	ON_EN_KILLFOCUS(IDC_EDR, OnKillfocusEdr)
	ON_EN_KILLFOCUS(IDC_EDKOI, OnKillfocusEdkoi)
	ON_EN_KILLFOCUS(IDC_EDNOI, OnKillfocusEdnoi)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDNUMOI, OnKillfocusEdnumoi)
	ON_CBN_KILLFOCUS(IDC_CMBA0, OnKillfocusCmba0)
	ON_CBN_KILLFOCUS(IDC_CMBDF, OnKillfocusCmbdf)
	ON_EN_KILLFOCUS(IDC_EDZD, OnKillfocusEdzd)
	ON_CBN_KILLFOCUS(IDC_CMBVSK, OnKillfocusCmbDN)
	ON_BN_CLICKED(IDC_56_66, On5666)
	ON_EN_KILLFOCUS(IDC_EDDN, OnKillfocusEddn)
	ON_EN_KILLFOCUS(IDC_EDSHSIN, OnKillfocusEdshsin)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHINCR1, OnChincr1)
	ON_BN_CLICKED(IDC_CHINCR2, OnChincr2)
	ON_BN_CLICKED(IDC_CHINCR3, OnChincr3)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CHVIKL1, OnChvikl1)
	ON_BN_CLICKED(IDC_CHVIKL2, OnChvikl2)
	ON_BN_CLICKED(IDC_CHVIKL3, OnChvikl3)
	ON_BN_CLICKED(IDC_CHVIKL4, OnChvikl4)
	ON_BN_CLICKED(IDC_CHZD_IKM_TSEK, OnCHZDIKMtsek)
	ON_EN_CHANGE(IDC_EDRMAX, OnChangeEdrmax)
	ON_BN_CLICKED(IDC_A0, OnA0)
	ON_EN_KILLFOCUS(IDC_EDDR1, OnKillfocusEddrn)
	ON_EN_KILLFOCUS(IDC_EDDR2, OnKillfocusEddrk)
	ON_EN_KILLFOCUS(IDC_EDDN1, OnKillfocusEddn1)
	ON_EN_KILLFOCUS(IDC_EDDN2, OnKillfocusEddn2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFAS_USBDlg message handlers

BOOL CFAS_USBDlg::OnInitDialog()
{

	GetDlgItem(IDC_SAVE)->PostMessage(BM_SETIMAGE, IMAGE_ICON, (long)
		AfxGetApp()->LoadIcon(IDI_SAVE));

	GetDlgItem(IDC_OPEN)->PostMessage(BM_SETIMAGE, IMAGE_ICON, (long)
		AfxGetApp()->LoadIcon(IDI_OPEN));

	CDialog::OnInitDialog();
	CButton* chb56_66;
	chb56_66 = (CButton*)GetDlgItem(IDC_56_66);
	chb56_66->SetCheck(1);
	tt1.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt2.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt3.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt4.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	tt11.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt12.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt13.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt14.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	tt21.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt22.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt23.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt24.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	tt31.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt32.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt33.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt34.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	tt41.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt42.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt43.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt44.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt45.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);
	tt46.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	tt47.Create(this, TTS_ALWAYSTIP | WS_CHILD | WS_VISIBLE);

	CEdit* edRn1, * edRn2, * edRn3, * edRn4;

	edRn1 = (CEdit*)GetDlgItem(IDC_EDRN1);
	tt1.AddTool(edRn1, hint1);

	edRn2 = (CEdit*)GetDlgItem(IDC_EDRN2);
	tt2.AddTool(edRn2, hint1);

	edRn3 = (CEdit*)GetDlgItem(IDC_EDRN3);
	tt3.AddTool(edRn3, hint1);

	edRn4 = (CEdit*)GetDlgItem(IDC_EDRN4);
	tt4.AddTool(edRn4, hint1);

	CEdit* edRk1, * edRk2, * edRk3, * edRk4;

	edRk1 = (CEdit*)GetDlgItem(IDC_EDRK1);
	tt11.AddTool(edRk1, hint2);

	edRk2 = (CEdit*)GetDlgItem(IDC_EDRK2);
	tt12.AddTool(edRk2, hint2);

	edRk3 = (CEdit*)GetDlgItem(IDC_EDRK3);
	tt13.AddTool(edRk3, hint2);

	edRk4 = (CEdit*)GetDlgItem(IDC_EDRK4);
	tt14.AddTool(edRk4, hint2);

	CEdit* ed1N1, * ed1N2, * ed1N3, * ed1N4;

	ed1N1 = (CEdit*)GetDlgItem(IDC_ED1N1);
	tt21.AddTool(ed1N1, hint3);

	ed1N2 = (CEdit*)GetDlgItem(IDC_ED1N2);
	tt22.AddTool(ed1N2, hint3);

	ed1N3 = (CEdit*)GetDlgItem(IDC_ED1N3);
	tt23.AddTool(ed1N3, hint3);

	ed1N4 = (CEdit*)GetDlgItem(IDC_ED1N4);
	tt24.AddTool(ed1N4, hint3);

	CEdit* ed2N1, * ed2N2, * ed2N3, * ed2N4;

	ed2N1 = (CEdit*)GetDlgItem(IDC_ED2N1);
	tt31.AddTool(ed2N1, hint4);

	ed2N2 = (CEdit*)GetDlgItem(IDC_ED2N2);
	tt32.AddTool(ed2N2, hint4);

	ed2N3 = (CEdit*)GetDlgItem(IDC_ED2N3);
	tt33.AddTool(ed2N3, hint4);

	ed2N4 = (CEdit*)GetDlgItem(IDC_ED2N4);
	tt34.AddTool(ed2N4, hint4);

	CEdit* edDN;

	edDN = (CEdit*)GetDlgItem(IDC_EDDN);
	tt41.AddTool(edDN, hint5);

	CButton* cSHP, * cKO, * cCO, * cNV, * c5666, * RMAX;
	cSHP = (CButton*)GetDlgItem(IDC_CHSHP);
	tt42.AddTool(cSHP, hint6);
	cKO = (CButton*)GetDlgItem(IDC_CHKO);
	tt43.AddTool(cKO, hint7);
	cCO = (CButton*)GetDlgItem(IDC_CHCO);
	tt44.AddTool(cCO, hint8);
	cNV = (CButton*)GetDlgItem(IDC_CHNV);
	tt45.AddTool(cNV, hint9);
	c5666 = (CButton*)GetDlgItem(IDC_56_66);
	tt46.AddTool(c5666, hint10);
	RMAX = (CButton*)GetDlgItem(IDC_EDRMAX);
	Get_Rmax();
	tt47.AddTool(RMAX, Rmax_in_device1);


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//------------------------------------------------------
//Устанавливаем checkbutton 5666 вкл по умолчеанию
	CButton* cb56_66;
	cb56_66 = (CButton*)GetDlgItem(IDC_56_66);
	cb56_66->SetCheck(1);
	//-------------------------------------------------------
	CComboBox* cb_A0, * cb_dN, * cb_DF;
	cb_A0 = (CComboBox*)GetDlgItem(IDC_CMBA0);
	cb_A0->SetCurSel(0);

	cb_dN = (CComboBox*)GetDlgItem(IDC_CMBVSK);
	cb_dN->SetCurSel(0);

	cb_DF = (CComboBox*)GetDlgItem(IDC_CMBDF);
	cb_DF->SetCurSel(4);
	CEdit* edNOI;
	edNOI = (CEdit*)GetDlgItem(IDC_EDNUMOI);
	edNOI->SetWindowText("0");
	edNOI->EnableWindow(false);

	//------------------------------------------------------
	//Вычисление Rmax 
	//m_cNumOI.GetWindowText(m_sNumOI);
	Get_Rmax();
	//--------------------------------------------------------
	bPusk = true;
	m_bPusk.LoadBitmap(IDB_PUSKU);
	HBITMAP hbitmap = (HBITMAP)m_bPusk.GetSafeHandle();
	m_bbut = (CBitmapButton*)GetDlgItem(ID_PUSK);
	m_bbut->SetBitmap(hbitmap);

	int numb = 0;
	int n = 0;
	char* pBuf[5];
	char buf1[64] = " ";
	char buf2[64] = " ";
	char buf3[64] = " ";
	char buf4[64] = " ";
	pBuf[0] = buf1;
	pBuf[1] = buf2;
	pBuf[2] = buf3;
	pBuf[3] = buf4;
	pBuf[4] = NULL;

	numb = m_ft_usb.Get_Number_Of_Devices();

	m_ft_usb.Get_All_Devices_List_By_SN(numb, pBuf);
	CString str, strSN, strDscr;
	strSN.Format("Количество устройств: \t%d\nSN1=%s\t\tSN2=%s\n", numb, buf1, buf2);
	m_ft_usb.Get_All_Devices_List_By_Description(numb, pBuf);
	//	strDscr.Format("Ds1=%s\tDs2=%s\n",buf1,buf2);
	//	str+=strSN;
	//	str+=strDscr;	


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CFAS_USBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFAS_USBDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		//	CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CDialog::OnPaint();
	}



}

HCURSOR CFAS_USBDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CFAS_USBDlg::OnOK()
{

	//	CDialog::OnOK();
}

void CFAS_USBDlg::OnCancel()
{

	//	CDialog::OnCancel();
}

void CFAS_USBDlg::OnClose()
{


	fclose(filelog);
	DestroyWindow();

	//CDialog::OnClose();
}

BOOL CFAS_USBDlg::IsCorrect(CString m_sEdit, int counter_razr)
{
	int count, count1;
	CString str;
	if (counter_razr == 4)
	{
		//исправить
		if (count1 > 8388608)
		{
			MessageBox(" Превышение 24 разрядов", "Инфо dN", MB_OK | MB_ICONHAND);
			return false;
		}

	}
	if (counter_razr == 2)
	{
		count1 = atoi(m_sEdit);
		if (count1 > 255)
		{
			MessageBox(" Превышение 255", "Инфо dN", MB_OK | MB_ICONHAND);
			return false;
		}

	}
	double cou1;
	char* ss;
	if (counter_razr == 3)
	{

		cou1 = strtod(m_sEdit, &ss);
		if (cou1 > 0.5 || cou1 < 0)
		{
			MessageBox("Выход за  интервал [0..0.5]", "Проверка шума", MB_OK | MB_ICONHAND);
			return false;
		}
	}
	if ('$' == m_sEdit.Left(1))
	{
		str = m_sEdit.SpanIncluding("$.ABCDEF1234567890");
		count = m_sEdit.GetLength();
		if ((count >= 6) && (counter_razr == 0))
		{
			MessageBox(" Превышение 16 разрядов", "Инфо", MB_OK | MB_ICONHAND);
			return false;
		}
	}
	else
	{
		str = m_sEdit.SpanIncluding(".1234567890");
		count = atoi(m_sEdit);
		if ((count >= 65536) && (counter_razr == 0))
		{
			MessageBox(" Превышение 16 разрядов", "Инфо", MB_OK | MB_ICONHAND);
			return false;
		}
	}
	if (str != m_sEdit)
	{
		MessageBox("Неверный символ", "Инфо", MB_OK | MB_ICONHAND);
		return false;
	}
	else {
		//	MessageBox("Correct record","Info",MB_OK|MB_ICONINFORMATION);
		return true;
	}
}

void CFAS_USBDlg::OnPusk()
{
	CString sTime;
	CTime t, td;
	td = t.GetCurrentTime();
	sTime = td.Format("%H:%M:%S\n");
	//-----------------------------------------------------------------------	



	//-----------------------------------------------------------------------	

	fputs(sTime, filelog);//Вставлям в log-файл время

	CComboBox* pcb;
	pcb = (CComboBox*)GetDlgItem(IDC_CMBA0);
	int cursel1 = pcb->GetCurSel();

	bPusk ^= 1;//кнопка пуск меняющая цвет

#ifndef _DEBUG
	if (!bPusk)
	{

		CButton* chb;
		chb = (CButton*)GetDlgItem(IDC_CHSHP);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHKO);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHCO);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHNV);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_56_66);
		chb->EnableWindow(false);

		chb = (CButton*)GetDlgItem(IDC_CHINCR1);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCR2);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCR3);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCR4);
		chb->EnableWindow(false);

		chb = (CButton*)GetDlgItem(IDC_CHINCOI1);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI2);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI3);
		chb->EnableWindow(false);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI4);
		chb->EnableWindow(false);



		m_bPusk.LoadBitmap(IDB_PUSKD);
		HBITMAP hbitmap = (HBITMAP)m_bPusk.GetSafeHandle();
		m_bbut = (CBitmapButton*)GetDlgItem(ID_PUSK);
		m_bbut->SetBitmap(hbitmap);
	}
	else
	{

		CButton* chb;
		chb = (CButton*)GetDlgItem(IDC_CHSHP);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHKO);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHCO);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHNV);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_56_66);
		chb->EnableWindow(true);

		chb = (CButton*)GetDlgItem(IDC_CHINCR1);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCR2);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCR3);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCR4);
		chb->EnableWindow(true);

		chb = (CButton*)GetDlgItem(IDC_CHINCOI1);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI2);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI3);
		chb->EnableWindow(true);
		chb = (CButton*)GetDlgItem(IDC_CHINCOI4);
		chb->EnableWindow(true);

		m_bPusk.LoadBitmap(IDB_PUSKU);
		HBITMAP hbitmap = (HBITMAP)m_bPusk.GetSafeHandle();
		m_bbut = (CBitmapButton*)GetDlgItem(ID_PUSK);
		m_bbut->SetBitmap(hbitmap);
	}
#endif
	//---------------Подготовка параметров------------------
	//	CString m_s;

	if (bPusk == false)
	{


		//Вычисляем Rmax
		Get_Rmax();

		//Формирование массива данных для обмена
		Forming_IOData();


		//---------------Запись в устройство--------------------
		int SearchingFAS = 0;
		int numb = m_ft_usb.Get_Number_Of_Devices();
		CString S_numb, SSS;
		S_numb.Format("%d", numb);
		SSS = "Обнаружено " + S_numb + " устройств\n";
		fputs(SSS, filelog);

		if (numb == 0)
		{
			fputs("Не обнаружено ни одного устройства\n", filelog);
		}
		else
		{
			ft_st = m_ft_usb.Get_All_Devices_List_By_SN(numb, pBuf_);
			if (ft_st == FT_OK)	fputs("1.Список устройств сформирован\n", filelog);
			for (int i = 0; i < numb; i++)

			{
				ft_st = m_ft_usb.Open_Device_by_SN(pBuf_[i]);//port B = 1
				if (ft_st == FT_OK)fputs("2.Устройство открыто\n", filelog);

				ft_st = m_ft_usb.Reset_Device();
				if (ft_st == FT_OK)fputs("3.Сброс устройства произведен\n", filelog);

				ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);
				if (ft_st == FT_OK)fputs("4.Очистка буферов произведена\n", filelog);

				ft_st = m_ft_usb.Set_Device_Timeouts(5000, 5000);
				if (ft_st == FT_OK)	fputs("5.Таймаут установлен(5000,5000)\n", filelog);

				BYTE rd80[4] = { 0x07,0x80,00,00 };

				int rez = m_ft_usb.Write_In_Device((LPBYTE)rd80, 4);//Запись команды чтение(07)регистра 80р 
				if (rez != 4)
				{
					fputs("6.Ошибка записи команды <чтение RG80>\n", filelog);
				}
				else
				{
					fputs("6.Запись команды <чтение RG80> завершена\n", filelog);
				}
				fputs("-------------------\n", filelog);


				//LPBYTE rfd;
				char rfd[256];
				int retByte;
				retByte = m_ft_usb.Read_From_Device(rfd);
				CString st1, st2;
				st2.Format("%X", rfd[0]);
				st1 = "7.Содержимое регистра 80h - " + st2 + "h\n";
				fputs(st1, filelog);
				CStatic* stFindFAS;
				stFindFAS = (CStatic*)GetDlgItem(IDC_STSEARCHFAS);
				if ((int)rfd[0] == 0x55)
				{
					stFindFAS->ShowWindow(SW_SHOW);
					SearchingFAS = 1;
					break;
				}
				else
				{
					stFindFAS->ShowWindow(SW_HIDE);
				}
				m_ft_usb.Close_Device();
			}
			if (SearchingFAS == 1)

			{

				CString sd;
				for (int j = 0; j < 129; j = j + 4)
				{

					if (j == 0)
					{
						sd.Format("%0.2X\n", IOD[j]);
						fputs(sd, filelog);
						j = 1;
					}
					if (j == 57)
					{
						sd.Format("%0.2X %0.2X %0.2X %0.2X %0.2X \n", IOD[j], IOD[j + 1], IOD[j + 2], IOD[j + 3], IOD[j + 4]);
						fputs(sd, filelog);
						j++;
					}
					else

					{
						sd.Format("%0.2X %0.2X %0.2X %0.2X \n", IOD[j], IOD[j + 1], IOD[j + 2], IOD[j + 3]);
						fputs(sd, filelog);
					}
				}
				fputs("-------------------\n", filelog);
				int retBytes = m_ft_usb.Write_In_Device((LPBYTE)IOD, 129);
				if (retBytes != 129)
				{
					fputs("8.НЕ все байты переданы \n", filelog);
				}
				else
				{
					fputs("8.Все байты переданы \n", filelog);
				}

			}
		}
		//	RunTimer=SetTimer(1,100,NULL);
		/*		MSG msg;
				int i1,i2;
			CString m_sRn1,m_sRk1,m_sdR1,m_sOIn1,m_sOIk1,
					m_sRn2,m_sRk2,m_sdR2,m_sOIn2,m_sOIk2, s_temp;
			CEdit *EdRn,* EdDR1;
			EdRn=(CEdit*)GetDlgItem(IDC_EDRN1);
			EdDR1=(CEdit*)GetDlgItem(IDC_EDDRN);
				while(-1)
				{
		EdRn->GetWindowText(m_sRn1);
		EdDR1->GetWindowText(m_sdR1);
		i1=atoi(m_sRn1);
		i2=atoi(m_sdR1);
		i1=i1+i2;
		s_temp.Format("%d",i1);
		EdRn->SetWindowText(s_temp);
		EdRn->UpdateData(false);


					if(::PeekMessage(&msg,NULL,0,0, PM_REMOVE))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}*/
		RunTimer = SetTimer(1, 1000, NULL);
	}
	else
	{
		int r = 0;
		cReset[0] = 0x91;

		ft_st = m_ft_usb.Reset_Device();
		r = m_ft_usb.Write_In_Device((LPBYTE)cReset, 1);

		if (r != 1)
		{
			fputs("СТОП.ошибка записи сброса при нажатии СТОП\n", filelog);
		}
		else
		{
			fputs("СТОП.Сброс\n", filelog);
		}
		ft_st = m_ft_usb.Reset_Device();
		ft_st = m_ft_usb.Close_Device();

		if (ft_st == FT_OK)
		{
			fputs("СТОП.Устройство закрыто \n", filelog);
		}
		else
		{
			fputs("СТОП.Ошибка при закрытии устройства \n", filelog);
		}

		KillTimer(RunTimer);
	}

}

void CFAS_USBDlg::OnKillfocusEdrn1()
{
	m_cRn1.GetWindowText(m_sRn1);
	IsCorrect(m_sRn1, 0);

	if (bPusk == false)
	{
		if (m_cRn1.GetModify() != 0)
		{
			Dyn_Record(0x0297, m_sRn1, "RG_Rn1(02h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdrk1()
{
	m_cRk1.GetWindowText(m_sRk1);
	IsCorrect(m_sRk1, 0);
	if (bPusk == false)
	{
		if (m_cRk1.GetModify() != 0)
		{
			Dyn_Record(0x0197, m_sRk1, "RG_Rk1(01h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd1n1()
{
	m_c1N1.GetWindowText(m_s1N1);
	IsCorrect(m_s1N1, 0);
	if (bPusk == false)
	{
		if (m_c1N1.GetModify() != 0)
		{
			Dyn_Record(0x0597, m_s1N1, "RG_1N1(05h)", 0);

		}
	}


}

void CFAS_USBDlg::OnKillfocusEd2n1()
{
	m_c2N1.GetWindowText(m_s2N1);
	IsCorrect(m_s2N1, 0);

	if (bPusk == false)
	{
		if (m_c2N1.GetModify() != 0)
		{
			Dyn_Record(0x0697, m_s2N1, "RG_2N1(06h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdf1()
{
	m_cF1.GetWindowText(m_sF1);
	IsCorrect(m_sF1, 0);
	if (bPusk == false)
	{
		if (m_cF1.GetModify() != 0)
		{
			Dyn_Record(0x0497, m_sF1, "RG_M1(04h)", 1);
		}
	}
}


void CFAS_USBDlg::OnKillfocusEdrn2()
{
	m_cRn2.GetWindowText(m_sRn2);
	IsCorrect(m_sRn2, 0);
	if (bPusk == false)
	{
		if (m_cRn2.GetModify() != 0)
		{
			Dyn_Record(0x0997, m_sRn2, "RG_Rn2(09h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdrk2()
{
	m_cRk2.GetWindowText(m_sRk2);
	IsCorrect(m_sRk2, 0);
	if (bPusk == false)
	{
		if (m_cRk2.GetModify() != 0)
		{
			Dyn_Record(0x0897, m_sRk2, "RG_Rk2(08h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd1n2()
{
	m_c1N2.GetWindowText(m_s1N2);
	IsCorrect(m_s1N2, 0);
	if (bPusk == false)
	{
		if (m_c1N2.GetModify() != 0)
		{
			Dyn_Record(0x0C97, m_s1N2, "RG_1N2(0Ch)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd2n2()
{
	m_c2N2.GetWindowText(m_s2N2);
	IsCorrect(m_s2N2, 0);
	if (bPusk == false)
	{
		if (m_c2N2.GetModify() != 0)
		{
			Dyn_Record(0x0D97, m_s2N2, "RG_2N2(0Dh)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdf2()
{
	m_cF2.GetWindowText(m_sF2);
	IsCorrect(m_sF2, 0);
	if (bPusk == false)
	{
		if (m_cF2.GetModify() != 0)
		{
			Dyn_Record(0x0B97, m_sF2, "RG_M2(0Bh)", 1);
		}
	}
}


void CFAS_USBDlg::OnKillfocusEdrn3()
{
	m_cRn3.GetWindowText(m_sRn3);
	IsCorrect(m_sRn3, 0);

	if (bPusk == false)
	{
		if (m_cRn3.GetModify() != 0)
		{
			Dyn_Record(0x1297, m_sRn3, "RG_Rn3(12h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdrk3()
{
	m_cRk3.GetWindowText(m_sRk3);
	IsCorrect(m_sRk3, 0);
	if (bPusk == false)
	{
		if (m_cRk3.GetModify() != 0)
		{
			Dyn_Record(0x1197, m_sRk3, "RG_Rk3(11h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd1n3()
{
	m_c1N3.GetWindowText(m_s1N3);
	IsCorrect(m_s1N3, 0);
	if (bPusk == false)
	{
		if (m_c1N3.GetModify() != 0)
		{
			Dyn_Record(0x1597, m_s1N3, "RG_1N3(15h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd2n3()
{
	m_c2N3.GetWindowText(m_s2N3);
	IsCorrect(m_s2N3, 0);
	if (bPusk == false)
	{
		if (m_c2N3.GetModify() != 0)
		{
			Dyn_Record(0x1697, m_s2N3, "RG_2N3(16h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdf3()
{
	m_cF3.GetWindowText(m_sF3);
	IsCorrect(m_sF3, 0);
	if (bPusk == false)
	{
		if (m_cF3.GetModify() != 0)
		{
			Dyn_Record(0x1497, m_sF3, "RG_M3(14h)", 1);
		}
	}
}


void CFAS_USBDlg::OnKillfocusEdrn4()
{
	m_cRn4.GetWindowText(m_sRn4);
	IsCorrect(m_sRn4, 0);
	if (bPusk == false)
	{
		if (m_cRn4.GetModify() != 0)
		{
			Dyn_Record(0x1997, m_sRn4, "RG_Rn4(19h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdrk4()
{
	m_cRk4.GetWindowText(m_sRk4);
	IsCorrect(m_sRk4, 0);
	if (bPusk == false)
	{
		if (m_cRk4.GetModify() != 0)
		{
			Dyn_Record(0x1897, m_sRk4, "RG_Rk4(18h)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEd1n4()
{
	m_c1N4.GetWindowText(m_s1N4);
	IsCorrect(m_s1N4, 0);
	if (bPusk == false)
	{
		if (m_c1N4.GetModify() != 0)
		{
			Dyn_Record(0x1C97, m_s1N4, "RG_1N4(1Ch)", 0);

		}
	}

}

void CFAS_USBDlg::OnKillfocusEd2n4()
{
	m_c2N4.GetWindowText(m_s2N4);
	IsCorrect(m_s2N4, 0);
	if (bPusk == false)
	{
		if (m_c2N4.GetModify() != 0)
		{
			Dyn_Record(0x1D97, m_s2N4, "RG_2N4(1Dh)", 0);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdf4()
{
	m_cF4.GetWindowText(m_sF4);
	IsCorrect(m_sF4, 0);
	if (bPusk == false)
	{
		if (m_cF4.GetModify() != 0)
		{
			Dyn_Record(0x1B97, m_sF4, "RG_M4(1Bh)", 1);
		}
	}
}

void CFAS_USBDlg::OnKillfocusEdrmax()
{
	//UpdateData(false);
	//m_cRmax.GetWindowText(m_sRmax);
	//IsCorrect(m_sRmax,4);

}

void CFAS_USBDlg::OnKillfocusEdr()
{
	m_cR.GetWindowText(m_sR);
	IsCorrect(m_sR, 3);
	int tc = atof(m_sR);
	tc = tc * 2047 / 0.5;
	CString scos;
	scos.Format("%f", tc);
	if (bPusk == false)
	{
		if (m_cR.GetModify() != 0)
		{
			Dyn_Record(0x0397, scos, "RG_SHCOS(03h)", 2);

		}
	}
}

void CFAS_USBDlg::OnKillfocusEdkoi()
{
	m_cKOI.GetWindowText(m_sKOI);
	IsCorrect(m_sKOI, 0);
}

void CFAS_USBDlg::OnKillfocusEdnoi()
{
	m_cNOI.GetWindowText(m_sNOI);
	IsCorrect(m_sNOI, 0);

}

void CFAS_USBDlg::OnKillfocusEdnumoi()
{
	m_cNumOI.GetWindowText(m_sNumOI);
	IsCorrect(m_sNumOI, 0);
	Get_Rmax();
	if (bPusk == false)
	{
		if (m_cNumOI.GetModify() != 0)
		{
			io_rg.nIO_RG[0] = 0x0A97;
			io_rg.nIO_RG[1] = Reverse_Hi_Lo_Bytes(m_sNumOI, 0);


			ft_st = m_ft_usb.Reset_Device();
			ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);

			int r = m_ft_usb.Write_In_Device((LPBYTE)io_rg.cIO_RG, 4);

			if (r != 4)
			{
				fputs("Ошибка записи в RG_OI(0Ah)", filelog);
			}
			else
			{
				fputs("Запись в RG_OI(0Ah) произведена\n", filelog);
			}
		}
	}
}

void CFAS_USBDlg::OnKillfocusCmba0()
{
	m_cNumOI.GetWindowText(m_sNumOI);
	Get_Rmax();

}

void CFAS_USBDlg::OnKillfocusCmbdf()
{
	m_cNumOI.GetWindowText(m_sNumOI);
	Get_Rmax();
	//------------------------------
	io_rg.nIO_RG[0] = 0x0F97;
	CComboBox* pcb1;
	pcb1 = (CComboBox*)GetDlgItem(IDC_CMBDF);

	int cursel2 = pcb1->GetCurSel();

	switch (cursel2)
	{
	case 0:io_rg.nIO_RG[1] = 0x0100; break;
	case 1:io_rg.nIO_RG[1] = 0x0200; break;
	case 2:io_rg.nIO_RG[1] = 0x0400; break;
	case 3:io_rg.nIO_RG[1] = 0x0800; break;
	case 4:io_rg.nIO_RG[1] = 0x1000; break;

	}

	if (bPusk == false)
	{

		ft_st = m_ft_usb.Reset_Device();
		ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);

		int r = m_ft_usb.Write_In_Device((LPBYTE)io_rg.cIO_RG, 4);

		if (r != 4)
		{
			fputs("Ошибка записи в RG_DF(0Fh)", filelog);
		}
		else
		{
			fputs("Запись в RG_DF(0Fh) произведена\n", filelog);
		}

	}

}

void CFAS_USBDlg::Get_Rmax()
{
	CButton* chb56_66;
	chb56_66 = (CButton*)GetDlgItem(IDC_56_66);
	int Rmax;
	float fRmax;
	CString sRm;
	CEdit* edRm;
	edRm = (CEdit*)GetDlgItem(IDC_EDRMAX);
	edRm->GetWindowText(sRm);
	fRmax = atof(sRm);
	int t = chb56_66->GetCheck();
	if (chb56_66->GetCheck())
	{

		Rmax = fRmax * pow(10, -6) / (15.0 * pow(10, -9));
		Rmax_in_device1.Format("Rmax = %d", Rmax);
		Rmax_in_device.Format("%d", Rmax);
	}
	else
	{
		Rmax = fRmax * pow(10, -6) / (18.0 * pow(10, -9));
		Rmax_in_device1.Format("Rmax = %d", Rmax);
		Rmax_in_device.Format("%d", Rmax);
	}
}

int CFAS_USBDlg::Forming_RGU()
{
	CButton* chb11, * chb12, * chb21, * chb22, * chb31, * chb32, * chb41, * chb42, * chbshp, * chbko, * chbco, * chbnv, * chb56_66, * chbSC;

	chb11 = (CButton*)GetDlgItem(IDC_CHINCR1);
	if (chb11->GetCheck())
	{
		regRGU = regRGU | 0x00000010;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFEF;
	}

	chb12 = (CButton*)GetDlgItem(IDC_CHSINCOS);
	if (chb12->GetCheck())
	{
		regRGU = regRGU | 0x00000020;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFDF;
	}

	chb21 = (CButton*)GetDlgItem(IDC_CHINCR2);
	if (chb21->GetCheck())
	{
		regRGU = regRGU | 0x00000040;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFFBF;
	}
	chb22 = (CButton*)GetDlgItem(IDC_CHINCOI2);
	if (chb22->GetCheck())
	{
		regRGU = regRGU | 0x00000080;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFF7F;
	}

	chb31 = (CButton*)GetDlgItem(IDC_CHINCR3);
	if (chb31->GetCheck())
	{
		regRGU = regRGU | 0x00000100;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFEFF;
	}
	chb32 = (CButton*)GetDlgItem(IDC_CHINCOI3);
	if (chb32->GetCheck())
	{
		regRGU = regRGU | 0x00000200;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFDFF;
	}
	chb41 = (CButton*)GetDlgItem(IDC_CHINCR4);
	if (chb41->GetCheck())
	{
		regRGU = regRGU | 0x00000400;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFBFF;
	}
	chb42 = (CButton*)GetDlgItem(IDC_CHINCOI4);
	if (chb42->GetCheck())
	{
		regRGU = regRGU | 0x00000800;
	}
	else
	{
		regRGU = regRGU & 0xFFFFF7FF;
	}
	chbnv = (CButton*)GetDlgItem(IDC_CHNV);
	if (chbnv->GetCheck())
	{
		regRGU = regRGU | 0x00000008;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFF7;
	}
	chbco = (CButton*)GetDlgItem(IDC_CHCO);
	if (chbco->GetCheck())
	{
		regRGU = regRGU | 0x00000004;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFFB;
	}

	chbko = (CButton*)GetDlgItem(IDC_A0);
	if (chbko->GetCheck())
	{
		regRGU = regRGU | 0x00000002;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFFD;
	}
	chbshp = (CButton*)GetDlgItem(IDC_CHSHP);
	if (chbshp->GetCheck())
	{
		regRGU = regRGU | 0x00000001;
	}
	else
	{
		regRGU = regRGU & 0xFFFFFFFE;
	}

	chb56_66 = (CButton*)GetDlgItem(IDC_56_66);
	if (chb56_66->GetCheck())
	{
		regRGU = regRGU | 0x00008000;
	}
	else
	{
		regRGU = regRGU & 0xFFFF7FFF;
	}

	int RGst, RGm, RGm_st, RGmst;
	RGst = regRGU >> 8;
	RGst = RGst & 0x00FF;
	RGm = regRGU & 0x00FF;
	RGm_st = RGm << 8;
	RGmst = RGm_st | RGst;

	return RGmst;
}


void CFAS_USBDlg::Forming_IOData()
{
	int R32, R32ST, R32ML;
	CString s_nul = "0";
	CButton* bRk1, * bRk2, * bRk3, * bRk4;
	bRk1 = (CButton*)GetDlgItem(IDC_CHVIKL1);
	bRk2 = (CButton*)GetDlgItem(IDC_CHVIKL2);
	bRk3 = (CButton*)GetDlgItem(IDC_CHVIKL3);
	bRk4 = (CButton*)GetDlgItem(IDC_CHVIKL4);
	//Rk1
	if (bRk1->GetCheck())
	{
		m_cRn1.GetWindowText(m_sRn1);
		io_d.nIO_Data[0] = WR_RG_RK1;
		io_d.nIO_Data[1] = Reverse_Hi_Lo_Bytes(m_sRn1, 0);

	}
	else
	{
		m_cRk1.GetWindowText(m_sRk1);
		io_d.nIO_Data[0] = WR_RG_RK1;
		io_d.nIO_Data[1] = Reverse_Hi_Lo_Bytes(m_sRk1, 0);
	}
	//Rн1
	m_cRn1.GetWindowText(m_sRn1);
	io_d.nIO_Data[2] = WR_RG_RN1;
	io_d.nIO_Data[3] = Reverse_Hi_Lo_Bytes(m_sRn1, 0);
	//R
	double t;
	CString transformCos;
	m_cR.GetWindowText(m_sR);
	t = atof(m_sR);
	t = t * 2047 / 0.5;
	transformCos.Format("%f", t);
	io_d.nIO_Data[4] = WR_RG_SHCOS;
	io_d.nIO_Data[5] = Reverse_Hi_Lo_Bytes(transformCos, 0);
	//M1
	m_cF1.GetWindowText(m_sF1);
	io_d.nIO_Data[6] = WR_RG_M1;
	io_d.nIO_Data[7] = Reverse_Hi_Lo_Bytes(m_sF1, 1);
	//1N1
	m_c1N1.GetWindowText(m_s1N1);
	io_d.nIO_Data[8] = WR_RG_1N1;
	io_d.nIO_Data[9] = Reverse_Hi_Lo_Bytes(m_s1N1, 0);
	//2N1
	m_c2N1.GetWindowText(m_s2N1);
	io_d.nIO_Data[10] = WR_RG_2N1;
	io_d.nIO_Data[11] = Reverse_Hi_Lo_Bytes(m_s2N1, 0);

	//RGU
	io_d.nIO_Data[12] = WR_RG_RGU;
	io_d.nIO_Data[13] = Forming_RGU();

	//Rk2
	if (bRk2->GetCheck())
	{
		m_cRn2.GetWindowText(m_sRn2);
		io_d.nIO_Data[14] = WR_RG_RK2;
		io_d.nIO_Data[15] = Reverse_Hi_Lo_Bytes(m_sRn2, 0);

	}
	else
	{
		m_cRk2.GetWindowText(m_sRk2);
		io_d.nIO_Data[14] = WR_RG_RK2;
		io_d.nIO_Data[15] = Reverse_Hi_Lo_Bytes(m_sRk2, 0);
	}
	//	m_cRk2.GetWindowText(m_sRk2);
	//	io_d.nIO_Data[14]=WR_RG_RK2;
	//	io_d.nIO_Data[15]=Reverse_Hi_Lo_Bytes(m_sRk2,0);
		//Rн2
	m_cRn2.GetWindowText(m_sRn2);
	io_d.nIO_Data[16] = WR_RG_RN2;
	io_d.nIO_Data[17] = Reverse_Hi_Lo_Bytes(m_sRn2, 0);
	//-----------------------dRn
	//	m_cdRn.GetWindowText(m_sdRn);
	io_d.nIO_Data[18] = WR_RG_dRN;
	io_d.nIO_Data[19] = Reverse_Hi_Lo_Bytes(s_nul, 0);//m_sdRn
//--------------------------
	//OI
	m_cNumOI.GetWindowText(m_sNumOI);
	io_d.nIO_Data[20] = WR_RG_OI;
	io_d.nIO_Data[21] = Reverse_Hi_Lo_Bytes(m_sNumOI, 0);
	//M2
	m_cF2.GetWindowText(m_sF2);
	io_d.nIO_Data[22] = WR_RG_M2;
	io_d.nIO_Data[23] = Reverse_Hi_Lo_Bytes(m_sF2, 1);
	//1N2
	m_c1N2.GetWindowText(m_s1N2);
	io_d.nIO_Data[24] = WR_RG_1N2;
	io_d.nIO_Data[25] = Reverse_Hi_Lo_Bytes(m_s1N2, 0);
	//2N2
	m_c2N2.GetWindowText(m_s2N2);
	io_d.nIO_Data[26] = WR_RG_2N2;
	io_d.nIO_Data[27] = Reverse_Hi_Lo_Bytes(m_s2N2, 0);
	//Rmax
	//m_cRmax.GetWindowText(m_sRmax);

	io_d.nIO_Data[28] = WR_RG_RMAX;
	R32 = Reverse_Hi_Lo_Bytes(Rmax_in_device, 3);
	R32ST = (R32 >> 16) & 0x0000FFFF;
	R32ML = R32 & 0x0000FFFF;
	io_d.nIO_Data[29] = R32ML;
	io_d.nIO_Data[30] = R32ST;

	//SHSIN
//	CComboBox *pcb1;
//	pcb1=(CComboBox*)GetDlgItem(IDC_CMBDF);
//	int count_DF=pcb1->GetCurSel();
	double t1;
	CString transformSin;
	m_cShSin.GetWindowText(m_sShSin);
	t1 = atof(m_sShSin);
	t1 = t1 * 2047 / 0.5;
	transformSin.Format("%f", t1);
	io_d.nIO_Data[31] = WR_RG_SHSIN;
	io_d.nIO_Data[32] = Reverse_Hi_Lo_Bytes(transformSin, 0);
	//	io_d.nIO_Data[30]=Forming_DF(count_DF);
		//Rk3
	if (bRk3->GetCheck())
	{
		m_cRn3.GetWindowText(m_sRn3);
		io_d.nIO_Data[33] = WR_RG_RK3;
		io_d.nIO_Data[34] = Reverse_Hi_Lo_Bytes(m_sRn3, 0);

	}
	else
	{
		m_cRk3.GetWindowText(m_sRk3);
		io_d.nIO_Data[33] = WR_RG_RK3;
		io_d.nIO_Data[34] = Reverse_Hi_Lo_Bytes(m_sRk3, 0);
	}

	//	m_cRk3.GetWindowText(m_sRk3);
	//	io_d.nIO_Data[31]=WR_RG_RK3;
	//	io_d.nIO_Data[32]=Reverse_Hi_Lo_Bytes(m_sRk3,0);
		//Rн3
	m_cRn3.GetWindowText(m_sRn3);
	io_d.nIO_Data[35] = WR_RG_RN3;
	io_d.nIO_Data[36] = Reverse_Hi_Lo_Bytes(m_sRn3, 0);
	//DN
	//CComboBox *pcb2;
	//pcb2=(CComboBox*)GetDlgItem(IDC_CMBVSK);
	//int count_DN=pcb2->GetCurSel();
	io_d.nIO_Data[37] = WR_RG_DN;
	io_d.nIO_Data[38] = Reverse_Hi_Lo_Bytes(m_seddN, 0);//Forming_DN(count_DN);
	//M3
	m_cF3.GetWindowText(m_sF3);
	io_d.nIO_Data[39] = WR_RG_M3;
	io_d.nIO_Data[40] = Reverse_Hi_Lo_Bytes(m_sF3, 1);
	//1N3
	m_c1N3.GetWindowText(m_s1N3);
	io_d.nIO_Data[41] = WR_RG_1N3;
	io_d.nIO_Data[42] = Reverse_Hi_Lo_Bytes(m_s1N3, 0);
	//2N3
	m_c2N3.GetWindowText(m_s2N3);
	io_d.nIO_Data[43] = WR_RG_2N3;
	io_d.nIO_Data[44] = Reverse_Hi_Lo_Bytes(m_s2N3, 0);
	//VR_ZD
	CButton* pButZD;
	pButZD = (CButton*)GetDlgItem(IDC_CHZD_IKM_TSEK);
	if (pButZD->GetCheck())
	{
		m_cZD.GetWindowText(m_sZD);
		io_d.nIO_Data[45] = WR_RG_ZDIKM;
		io_d.nIO_Data[46] = Reverse_Hi_Lo_Bytes(m_sZD, 0);
	}
	else
	{
		m_cZD.GetWindowText(m_sZD);
		int izdv;
		float fzdv = atof(m_sZD);
		izdv = fzdv * 1000.0 / 15.0 + 1.0;
		CString s_zdv;
		s_zdv.Format("%d", izdv);
		io_d.nIO_Data[45] = WR_RG_ZDIKM;
		io_d.nIO_Data[46] = Reverse_Hi_Lo_Bytes(s_zdv, 0);
	}


	//Rk4
	if (bRk4->GetCheck())
	{
		m_cRn4.GetWindowText(m_sRn4);
		io_d.nIO_Data[47] = WR_RG_RK4;
		io_d.nIO_Data[48] = Reverse_Hi_Lo_Bytes(m_sRn4, 0);

	}
	else
	{
		m_cRk4.GetWindowText(m_sRk4);
		io_d.nIO_Data[47] = WR_RG_RK4;
		io_d.nIO_Data[48] = Reverse_Hi_Lo_Bytes(m_sRk4, 0);
	}
	//	m_cRk4.GetWindowText(m_sRk4);
	//	io_d.nIO_Data[45]=WR_RG_RK4;
	//	io_d.nIO_Data[46]=Reverse_Hi_Lo_Bytes(m_sRk4,0);
		//Rн4
	m_cRn4.GetWindowText(m_sRn4);
	io_d.nIO_Data[49] = WR_RG_RN4;
	io_d.nIO_Data[50] = Reverse_Hi_Lo_Bytes(m_sRn4, 0);
	//---------------------------------------------dRk
	//	m_cdRk.GetWindowText(m_sdRk);
	io_d.nIO_Data[51] = WR_RG_dRk;
	io_d.nIO_Data[52] = Reverse_Hi_Lo_Bytes(s_nul, 0);//m_sdRk
//-----------------------------------------------------
	//M4
	m_cF4.GetWindowText(m_sF4);
	io_d.nIO_Data[53] = WR_RG_M4;
	io_d.nIO_Data[54] = Reverse_Hi_Lo_Bytes(m_sF4, 1);
	//1N4
	m_c1N4.GetWindowText(m_s1N4);
	io_d.nIO_Data[55] = WR_RG_1N4;
	io_d.nIO_Data[56] = Reverse_Hi_Lo_Bytes(m_s1N4, 0);
	//2N4
	m_c2N4.GetWindowText(m_s2N4);
	io_d.nIO_Data[57] = WR_RG_2N4;
	io_d.nIO_Data[58] = Reverse_Hi_Lo_Bytes(m_s2N4, 0);
	//---------------------------------------------------d1N
	//	m_cdN1.GetWindowText(m_sdN1);
	io_d.nIO_Data[59] = WR_RG_d1N;
	io_d.nIO_Data[60] = Reverse_Hi_Lo_Bytes(s_nul, 0);//m_sdN1
	//d2N
//	m_cdN2.GetWindowText(m_sdN2);
	io_d.nIO_Data[61] = WR_RG_d2N;
	io_d.nIO_Data[62] = Reverse_Hi_Lo_Bytes(s_nul, 0);//m_sdN2
//------------------------------------------------------
	//PUSK
	io_d.nIO_Data[63] = 0x0092;
	io_d.nIO_Data[64] = 0x0000;


	IOD[0] = 0x91;//SBROS
	int r24 = 0;
	for (int i = 0; i < 129; i++)
	{
		if (i == 61) { r24 = 1; }

		IOD[i + 1] = io_d.cIO_Data[i + r24];
	}

}

unsigned int CFAS_USBDlg::Reverse_Hi_Lo_Bytes(CString sData, int priznak)
{
	//priznak=1 формирование М1, М2, М3, М4
	//priznak=2 формирование R
	//
	int Dm, Dst, Dm_st, Dmst, D, Drez, Dsr;
	int sel;

	char* stopstring, * string;

	if ('$' == sData.Left(1))
	{
		sData = sData.Right(sData.GetLength() - 1);
		string = sData.GetBuffer(sData.GetLength());
		D = strtoul(string, &stopstring, 16);
	}
	else
	{
		D = atoi(sData);
	}
	switch (priznak)
	{
	case 0:
		Dst = D >> 8;
		Dst = Dst & 0x00FF;
		Dm = D & 0x00FF;
		Dm_st = Dm << 8;
		Dmst = Dm_st | Dst;
		break;
	case 1:
		//D/=7;
		Dst = D >> 8;
		Dst = Dst & 0x00FF;
		Dm = D & 0x00FF;
		Dm_st = Dm << 8;
		Dmst = Dm_st | Dst;

		break;
	case 2:
		CComboBox * cmb;
		cmb = (CComboBox*)GetDlgItem(IDC_CMBDF);
		sel = cmb->GetCurSel();
		switch (sel)
		{
		case 0:D = D * 16 - 2; break;
		case 1:D = D * 8 - 2; break;
		case 2:D = D * 4 - 2; break;
		case 3:D = D * 2 - 2; break;
		case 4:D = D * 1 - 2; break;
		}

		Dst = D >> 8;
		Dst = Dst & 0x00FF;
		Dm = D & 0x00FF;
		Dm_st = Dm << 8;
		Dmst = Dm_st | Dst;

		break;

	case 3:
		Dst = D >> 16;
		Dst = Dst & 0x00FF;
		Dsr = D & 0x00FF00;
		Dm = D & 0x0000FF;
		Drez = Dm << 16;
		Drez = Drez | Dsr;
		Dmst = Drez | Dst;

		break;
	}



	/*
		if(priznak==1)D/=7;

		if(priznak==2)
		{
		CComboBox *cmb;
		cmb=(CComboBox*)GetDlgItem(IDC_CMBDF);
		int sel=cmb->GetCurSel();
		switch(sel)
		{
		case 0:D=D*16-2;break;
		case 1:D=D*8-2;break;
		case 2:D=D*4-2;break;
		case 3:D=D*2-2;break;
		case 4:D=D*1-2;break;

		}
		}

		Dst=D>>8;
		Dst=Dst&0x00FF;
		Dm=D&0x00FF;
		Dm_st=Dm<<8;
		Dmst=Dm_st|Dst;

	//24разр
		Dst=D>>16;
				Dst=Dst&0x00FF;
				Dsr=D&0x00FF00;
				Dm=D&0x0000FF;
				Drez=Dm<<16;
				Drez=Drez|Dsr;
				Dmst=Drez|Dst;
				break;
	*/


	return Dmst;
}

unsigned __int16 CFAS_USBDlg::Forming_DF(int count)
{
	int DF_data;

	switch (count)
	{
	case 0: DF_data = 0x0100; break;
	case 1: DF_data = 0x0200; break;
	case 2: DF_data = 0x0400; break;
	case 3: DF_data = 0x0800; break;
	case 4: DF_data = 0x1000; break;
	}
	return DF_data;
}

unsigned __int16 CFAS_USBDlg::Forming_DN(int Count)
{
	int DN_data;
	/*
	switch(Count)
	{
	case 0: DN_data=0x0100; break;
	case 1: DN_data=0x0200; break;
	case 2: DN_data=0x0400; break;
	case 3: DN_data=0x0800; break;
	case 4: DN_data=0x1000; break;
	case 5: DN_data=0x1100; break;
	case 6: DN_data=0x1200; break;
	case 7: DN_data=0x1400; break;
	case 8: DN_data=0x1800; break;
	}

	*/
	return DN_data;

}

BOOL CFAS_USBDlg::DestroyWindow()
{
	delete(tt1);
	delete(tt2);
	delete(tt3);
	delete(tt4);

	delete(tt11);
	delete(tt12);
	delete(tt13);
	delete(tt14);

	delete(tt21);
	delete(tt22);
	delete(tt23);
	delete(tt24);

	delete(tt31);
	delete(tt32);
	delete(tt33);
	delete(tt34);

	delete(tt41);
	delete(tt42);
	delete(tt43);
	delete(tt44);
	delete(tt45);
	delete(tt46);
	delete(tt47);
	return CDialog::DestroyWindow();
}

void CFAS_USBDlg::Dyn_Record(int numb_rg, CString sEdit, CString sRG, int priznakM)
{
	io_rg.nIO_RG[0] = numb_rg;
	io_rg.nIO_RG[1] = Reverse_Hi_Lo_Bytes(sEdit, priznakM);

	ft_st = m_ft_usb.Reset_Device();
	ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);

	int r = m_ft_usb.Write_In_Device((LPBYTE)io_rg.cIO_RG, 4);

	if (r != 4)
	{
		fputs("Ошибка записи в " + sRG + "/n", filelog);
	}
	else
	{
		fputs("Запись в " + sRG + "произведена\n", filelog);
	}


}

void CFAS_USBDlg::OnKillfocusEdzd()
{
	CButton* pButZD;
	pButZD = (CButton*)GetDlgItem(IDC_CHZD_IKM_TSEK);
	if (pButZD->GetCheck())
	{
		int tek = atoi(m_sZD);
		if (tek < 0 || tek>53334)
		{
			AfxMessageBox("Текущее значение вне диапазона [0..53334]");
		}

		m_cZD.GetWindowText(m_sZD);
		if (bPusk == false)
		{
			if (m_cZD.GetModify() != 0)
			{
				Dyn_Record(0x1797, m_sZD, "RG_ZD_IKM(17h)", 0);

			}
		}
	}
	else
	{
		m_cZD.GetWindowText(m_sZD);
		int tek = atoi(m_sZD);
		if (tek < 0 || tek>800)
		{
			AfxMessageBox("Текущее значение вне диапазона [0..800]");
		}
		tek = tek * 1000 / 15 + 1;
		CString sZD;
		sZD.Format("%d", tek);
		if (bPusk == false)
		{
			if (m_cZD.GetModify() != 0)
			{
				Dyn_Record(0x1797, sZD, "RG_ZD_IKM(17h)", 0);

			}
		}
	}
}

void CFAS_USBDlg::OnKillfocusCmbDN()
{
	io_rg.nIO_RG[0] = 0x1397;
	CComboBox* pcb2;
	pcb2 = (CComboBox*)GetDlgItem(IDC_CMBVSK);

	int curselDN = pcb2->GetCurSel();
	switch (curselDN)
	{
	case 0: io_rg.nIO_RG[1] = 0x0100; break;
	case 1: io_rg.nIO_RG[1] = 0x0200; break;
	case 2: io_rg.nIO_RG[1] = 0x0400; break;
	case 3: io_rg.nIO_RG[1] = 0x0800; break;
	case 4: io_rg.nIO_RG[1] = 0x1000; break;
	case 5: io_rg.nIO_RG[1] = 0x1100; break;
	case 6: io_rg.nIO_RG[1] = 0x1200; break;
	case 7: io_rg.nIO_RG[1] = 0x1400; break;
	case 8: io_rg.nIO_RG[1] = 0x1800; break;
	}
	if (bPusk == false)
	{

		ft_st = m_ft_usb.Reset_Device();
		ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);

		int r = m_ft_usb.Write_In_Device((LPBYTE)io_rg.cIO_RG, 4);

		if (r != 4)
		{
			fputs("Ошибка записи в RG_DN(13h)", filelog);
		}
		else
		{
			fputs("Запись в RG_DN(13h) произведена\n", filelog);
		}

	}

}

void CFAS_USBDlg::On5666()
{
	Get_Rmax();
	CButton* btn;
	btn = (CButton*)GetDlgItem(IDC_56_66);
	Get_Rmax();
	if (btn->GetCheck())
	{
		btn->SetWindowText("66MHz");
	}
	else
	{

		btn->SetWindowText("56MHz");

	}
	CButton* RMAX;
	RMAX = (CButton*)GetDlgItem(IDC_EDRMAX);
	Get_Rmax();
	tt47.AddTool(RMAX, Rmax_in_device1);
}

void CFAS_USBDlg::OnKillfocusEddn()
{
	m_ceddN.GetWindowText(m_seddN);
	IsCorrect(m_seddN, 2);

	if (bPusk == false)
	{
		if (m_ceddN.GetModify() != 0)
		{
			io_rg.nIO_RG[0] = 0x0D97;
			io_rg.nIO_RG[1] = Reverse_Hi_Lo_Bytes(m_seddN, 0);


			ft_st = m_ft_usb.Reset_Device();
			ft_st = m_ft_usb.Purge_Device(FT_PURGE_RX | FT_PURGE_TX);

			int r = m_ft_usb.Write_In_Device((LPBYTE)io_rg.cIO_RG, 4);

			if (r != 4)
			{
				fputs("Ошибка записи в RG_dN(0Dh)", filelog);
			}
			else
			{
				fputs("Запись в RG_dN(0Dh) произведена\n", filelog);
			}
		}
	}
}

void CFAS_USBDlg::OnKillfocusEdshsin()
{
	m_cShSin.GetWindowText(m_sShSin);
	IsCorrect(m_sShSin, 3);
	int ts = atof(m_sShSin);
	ts = ts * 2047 / 0.5;
	CString ssin;
	ssin.Format("%f", ts);
	if (bPusk == false)
	{
		if (m_cShSin.GetModify() != 0)
		{
			Dyn_Record(0x0F97, ssin, "RG_SHSIN(0Fh)", 2);

		}
	}
}

HBRUSH CFAS_USBDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int numb = m_ft_usb.Get_Number_Of_Devices();
	CString str, str_nustr;
	CStatic* stc;
	stc = (CStatic*)GetDlgItem(IDC_STSEARCH);

	if (pWnd->GetDlgCtrlID() == IDC_STSEARCH)
	{
		if (numb > 0)
		{
			pDC->SetTextColor(RGB(0, 255, 0));

			pDC->SetBkColor(RGB(0, 128, 0));
			str_nustr.Format("%d", numb);
			str = "Подключенных устройств: " + str_nustr;
			stc->SetWindowText(str);
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));

			pDC->SetBkColor(RGB(128, 0, 0));
			str = "Устройство не подключено";
			stc->SetWindowText(str);
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_STSEARCHFAS)

	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CFAS_USBDlg::OnChincr1()
{
	CButton* chb1, * chb2, * chb3;

	chb1 = (CButton*)GetDlgItem(IDC_CHINCR1);
	chb2 = (CButton*)GetDlgItem(IDC_CHINCR2);
	chb3 = (CButton*)GetDlgItem(IDC_CHINCR3);

	if (chb1->GetCheck())
	{
		chb2->SetCheck(0);
		chb3->SetCheck(0);
	}

}

void CFAS_USBDlg::OnChincr2()
{
	CButton* chb1, * chb2, * chb3;

	chb1 = (CButton*)GetDlgItem(IDC_CHINCR1);
	chb2 = (CButton*)GetDlgItem(IDC_CHINCR2);
	chb3 = (CButton*)GetDlgItem(IDC_CHINCR3);

	if (chb2->GetCheck())
	{
		chb1->SetCheck(0);
		chb3->SetCheck(0);
	}
}

void CFAS_USBDlg::OnChincr3()
{
	CButton* chb1, * chb2, * chb3;

	chb1 = (CButton*)GetDlgItem(IDC_CHINCR1);
	chb2 = (CButton*)GetDlgItem(IDC_CHINCR2);
	chb3 = (CButton*)GetDlgItem(IDC_CHINCR3);

	if (chb3->GetCheck())
	{
		chb1->SetCheck(0);
		chb2->SetCheck(0);
	}
}

void CFAS_USBDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	tt1.Activate(true);
	tt2.Activate(true);
	tt3.Activate(true);
	tt4.Activate(true);

	tt11.Activate(true);
	tt12.Activate(true);
	tt13.Activate(true);
	tt14.Activate(true);

	tt21.Activate(true);
	tt22.Activate(true);
	tt23.Activate(true);
	tt24.Activate(true);

	tt31.Activate(true);
	tt32.Activate(true);
	tt33.Activate(true);
	tt34.Activate(true);

	tt41.Activate(true);
	tt42.Activate(true);
	tt43.Activate(true);
	tt44.Activate(true);
	tt45.Activate(true);
	tt46.Activate(true);
	tt47.Activate(true);
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CFAS_USBDlg::PreTranslateMessage(MSG* pMsg)
{
	tt1.RelayEvent(pMsg);
	tt2.RelayEvent(pMsg);
	tt3.RelayEvent(pMsg);
	tt4.RelayEvent(pMsg);

	tt11.RelayEvent(pMsg);
	tt12.RelayEvent(pMsg);
	tt13.RelayEvent(pMsg);
	tt14.RelayEvent(pMsg);

	tt21.RelayEvent(pMsg);
	tt22.RelayEvent(pMsg);
	tt23.RelayEvent(pMsg);
	tt24.RelayEvent(pMsg);

	tt31.RelayEvent(pMsg);
	tt32.RelayEvent(pMsg);
	tt33.RelayEvent(pMsg);
	tt34.RelayEvent(pMsg);


	tt41.RelayEvent(pMsg);
	tt42.RelayEvent(pMsg);
	tt43.RelayEvent(pMsg);
	tt44.RelayEvent(pMsg);
	tt45.RelayEvent(pMsg);
	tt46.RelayEvent(pMsg);
	tt47.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CFAS_USBDlg::OnTimer(UINT nIDEvent)
{
	//	MSG msg;
	int i1, i2, i3, i4,
		j1, j2, j3, j4,
		k1, k2, k3, k4;
	CString m_sRn1, m_sRk1, m_sdR1, m_sOIn1, m_sDOI1,
		m_sRn2, m_sRk2, m_sdR2, m_sOIn2, m_sDOI2, s_temp1, s_temp2, s_temp3, s_temp4, s_temp5, s_temp6;
	CEdit* EdRn1, * EdDR1, * EdRk1, * EdOIn1, * EdRn2, * EdDR2, * EdRk2, * EdOIn2, * EdDOI1, * EdDOI2;
	EdRn1 = (CEdit*)GetDlgItem(IDC_EDRN1);
	EdDR1 = (CEdit*)GetDlgItem(IDC_EDDR1);
	EdRk1 = (CEdit*)GetDlgItem(IDC_EDRK1);

	EdRn2 = (CEdit*)GetDlgItem(IDC_EDRN2);
	EdDR2 = (CEdit*)GetDlgItem(IDC_EDDR2);
	EdRk2 = (CEdit*)GetDlgItem(IDC_EDRK2);

	EdOIn1 = (CEdit*)GetDlgItem(IDC_ED1N1);
	EdOIn2 = (CEdit*)GetDlgItem(IDC_ED1N2);
	EdDOI1 = (CEdit*)GetDlgItem(IDC_EDDN1);
	EdDOI2 = (CEdit*)GetDlgItem(IDC_EDDN2);


	EdRn1->GetWindowText(m_sRn1);
	EdRk1->GetWindowText(m_sRk1);
	EdDR1->GetWindowText(m_sdR1);

	EdRn2->GetWindowText(m_sRn2);
	EdRk2->GetWindowText(m_sRk2);
	EdDR2->GetWindowText(m_sdR2);


	EdOIn1->GetWindowText(m_sOIn1);
	EdOIn2->GetWindowText(m_sOIn2);
	EdDOI1->GetWindowText(m_sDOI1);
	EdDOI2->GetWindowText(m_sDOI2);

	i1 = atoi(m_sRn1);
	j1 = atoi(m_sRk1);
	i2 = atoi(m_sdR1);
	i3 = atoi(m_sRn2);
	j3 = atoi(m_sRk2);
	i4 = atoi(m_sdR2);

	k1 = atoi(m_sOIn1);
	k2 = atoi(m_sOIn2);
	k3 = atoi(m_sDOI1);
	k4 = atoi(m_sDOI2);

	i1 = i1 + i2;
	i3 = i3 + i4;
	j1 = j1 + i2;
	j3 = j3 + i4;
	k1 = k1 + k3;
	k2 = k2 + k4;

	s_temp1.Format("%d", i1);
	EdRn1->SetWindowText(s_temp1);
	EdRn1->UpdateData(false);
	s_temp2.Format("%d", i3);
	EdRn2->SetWindowText(s_temp2);
	EdRn2->UpdateData(false);

	s_temp3.Format("%d", j1);
	EdRk1->SetWindowText(s_temp3);
	EdRk1->UpdateData(false);
	s_temp4.Format("%d", j3);
	EdRk2->SetWindowText(s_temp4);
	EdRk2->UpdateData(false);

	s_temp5.Format("%d", k1);
	EdOIn1->SetWindowText(s_temp5);
	EdOIn1->UpdateData(false);
	s_temp6.Format("%d", k2);
	EdOIn2->SetWindowText(s_temp6);
	EdOIn2->UpdateData(false);



	Dyn_Record(0x0297, s_temp1, "RG_Rn1(02h)", 0);
	Dyn_Record(0x0997, s_temp2, "RG_Rn2(09h)", 0);
	Dyn_Record(0x0197, m_sRk1, "RG_Rk1(01h)", 0);
	Dyn_Record(0x0897, m_sRk2, "RG_Rk2(08h)", 0);
	Dyn_Record(0x0597, m_s1N1, "RG_1N1(05h)", 0);
	Dyn_Record(0x0C97, m_s1N2, "RG_1N2(0Ch)", 0);

	CDialog::OnTimer(nIDEvent);
}

void CFAS_USBDlg::OnOpen()
{
	FILE* foutOP;
	CString sFilter = "Файлы параметров(*.cfg)|*.cfg|Все файлы(*.*)||", FileName, str_tmp;
	char cRn[6], cRk[6], cN1[6], cN2[6], cF[6];

	CFileDialog fdlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)sFilter, this);
	if (fdlg.DoModal() == IDOK)
	{
		FileName = fdlg.GetPathName();

		foutOP = fopen(FileName, "r");

		//цель1-----------------------------------
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sRn1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sRk1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		m_s1N1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN2, 7, foutOP);
		str_tmp = cN2;
		m_s2N1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cF, 7, foutOP);
		str_tmp = cF;
		m_sF1 = str_tmp.Left(str_tmp.GetLength() - 1);

		//цель2-----------------------------------
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sRn2 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sRk2 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		m_s1N2 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN2, 7, foutOP);
		str_tmp = cN2;
		m_s2N2 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cF, 7, foutOP);
		str_tmp = cF;
		m_sF2 = str_tmp.Left(str_tmp.GetLength() - 1);

		//цель3-----------------------------------
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sRn3 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sRk3 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		m_s1N3 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN2, 7, foutOP);
		str_tmp = cN2;
		m_s2N3 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cF, 7, foutOP);
		str_tmp = cF;
		m_sF3 = str_tmp.Left(str_tmp.GetLength() - 1);

		//цель4-----------------------------------
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sRn4 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sRk4 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		m_s1N4 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN2, 7, foutOP);
		str_tmp = cN2;
		m_s2N4 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cF, 7, foutOP);
		str_tmp = cF;
		m_sF4 = str_tmp.Left(str_tmp.GetLength() - 1);
		//NumOI, dN,Shum Sin Cos------------------
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_seddN = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sNumOI = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		str_tmp.GetLength();
		m_sR = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cF, 7, foutOP);
		str_tmp = cF;
		m_sShSin = str_tmp.Left(str_tmp.GetLength() - 1);
		//--248--------------------------------
		CButton* rb1, * rb2, * rb3, * rgu1, * rgu2, * rgu3, * rgu4, * rgu5, * rgu6, * rgu7;
		rb1 = (CButton*)GetDlgItem(IDC_CHINCR1);
		rb2 = (CButton*)GetDlgItem(IDC_CHINCR2);
		rb3 = (CButton*)GetDlgItem(IDC_CHINCR3);
		rgu1 = (CButton*)GetDlgItem(IDC_CHSHP);
		rgu2 = (CButton*)GetDlgItem(IDC_CHKO);
		rgu3 = (CButton*)GetDlgItem(IDC_CHCO);
		rgu4 = (CButton*)GetDlgItem(IDC_CHNV);
		rgu5 = (CButton*)GetDlgItem(IDC_56_66);
		rgu6 = (CButton*)GetDlgItem(IDC_CHSINCOS);
		rgu7 = (CButton*)GetDlgItem(IDC_CHSINCOS);
		int t;

		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rb1->SetCheck(1);
		}
		else
		{
			rb1->SetCheck(0);
		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rb2->SetCheck(1);
		}
		else
		{
			rb2->SetCheck(0);
		}

		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rb3->SetCheck(1);
		}
		else
		{
			rb3->SetCheck(0);
		}
		//RGU-------------------------------------
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rgu1->SetCheck(1);
		}
		else
		{
			rgu1->SetCheck(0);
		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rgu2->SetCheck(1);
		}
		else
		{
			rgu2->SetCheck(0);
		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rgu3->SetCheck(1);
		}
		else
		{
			rgu3->SetCheck(0);
		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		if (t == 1)
		{
			rgu4->SetCheck(1);
		}
		else
		{
			rgu4->SetCheck(0);
		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);
		CEdit* pEdRmax;
		pEdRmax = (CEdit*)GetDlgItem(IDC_EDRMAX);
		if (t == 1)
		{
			rgu5->SetCheck(1);
			//	m_sRmax="65541";
			//	m_cRmax.SetWindowText(m_sRmax);
			rgu5->SetWindowText("66MHz");
		}
		else
		{
			rgu5->SetCheck(0);
			//	m_sRmax="56622";
			//	m_cRmax.SetWindowText(m_sRmax);
			rgu5->SetWindowText("56MHz");
		}

		fgets(cRn, 7, foutOP);
		t = atoi(cRn);

		if (t == 1)
		{
			rgu6->SetCheck(1);
		}
		else
		{
			rgu6->SetCheck(0);

		}
		fgets(cRn, 7, foutOP);
		t = atoi(cRn);

		if (t == 1)
		{
			rgu7->SetCheck(1);
			rgu7->SetWindowText("Альфа 0 Внутр");
		}
		else
		{
			rgu7->SetCheck(0);
			rgu7->SetWindowText("Альфа 0 Внеш");


		}

		char ch_zd[6];
		fgets(ch_zd, 7, foutOP);
		t = atoi(ch_zd);
		CButton* sZDIKM;
		sZDIKM = (CButton*)GetDlgItem(IDC_CHZD_IKM_TSEK);
		if (t == 1)
		{
			sZDIKM->SetCheck(1);
			sZDIKM->SetWindowText("Сдвиг ИКМ, такты");
		}
		else
		{
			sZDIKM->SetCheck(0);
			sZDIKM->SetWindowText("Сдвиг ИКМ, мксек");
		}

		char cZD_IKM[11];
		fgets(cZD_IKM, 11, foutOP);
		str_tmp = cZD_IKM;
		m_sZD = str_tmp.Left(str_tmp.GetLength() - 1);

		//NumOI, dR1, dR2, dR3, dR4
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sRmax = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRn, 7, foutOP);
		str_tmp = cRn;
		m_sdR1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cRk, 7, foutOP);
		str_tmp = cRk;
		m_sdR2 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN1, 7, foutOP);
		str_tmp = cN1;
		m_sdN1 = str_tmp.Left(str_tmp.GetLength() - 1);
		fgets(cN2, 7, foutOP);
		str_tmp = cN2;
		m_sdN2 = str_tmp.Left(str_tmp.GetLength() - 1);


		fclose(foutOP);
		UpdateData(false);
	}


}

void CFAS_USBDlg::OnSave()
{
	FILE* foutSV;
	UpdateData();
	CString sFilter = "Файлы параметров(*.cfg)|*.cfg|Все файлы(*.*)||", FileNameSV;

	CFileDialog fdlg(FALSE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)sFilter, this);
	if (fdlg.DoModal() == IDOK)
	{

		FileNameSV = fdlg.GetPathName();
		if (FileNameSV.Find(".") == -1)
		{
			FileNameSV = FileNameSV + ".cfg";
		}
		foutSV = fopen(FileNameSV, "w");
		//цель1-----------------------------------			
		fputs(m_sRn1 + "\n", foutSV);
		fputs(m_sRk1 + "\n", foutSV);
		fputs(m_s1N1 + "\n", foutSV);
		fputs(m_s2N1 + "\n", foutSV);
		fputs(m_sF1 + "\n", foutSV);
		//цель2-----------------------------------
		fputs(m_sRn2 + "\n", foutSV);
		fputs(m_sRk2 + "\n", foutSV);
		fputs(m_s1N2 + "\n", foutSV);
		fputs(m_s2N2 + "\n", foutSV);
		fputs(m_sF2 + "\n", foutSV);
		//цель3-----------------------------------
		fputs(m_sRn3 + "\n", foutSV);
		fputs(m_sRk3 + "\n", foutSV);
		fputs(m_s1N3 + "\n", foutSV);
		fputs(m_s2N3 + "\n", foutSV);
		fputs(m_sF3 + "\n", foutSV);
		//цель4-----------------------------------
		fputs(m_sRn4 + "\n", foutSV);
		fputs(m_sRk4 + "\n", foutSV);
		fputs(m_s1N4 + "\n", foutSV);
		fputs(m_s2N4 + "\n", foutSV);
		fputs(m_sF4 + "\n", foutSV);
		//NumOI, dN, шум Cos,Sin------------------
		fputs(m_seddN + "\n", foutSV);
		fputs(m_sNumOI + "\n", foutSV);
		fputs(m_sR + "\n", foutSV);//ShCos
		fputs(m_sShSin + "\n", foutSV);

		CButton* rb1, * rb2, * rb3, * rgu1, * rgu2, * rgu3, * rgu4, * rgu5, * rgu6, * rgu7;
		rb1 = (CButton*)GetDlgItem(IDC_CHINCR1);
		rb2 = (CButton*)GetDlgItem(IDC_CHINCR2);
		rb3 = (CButton*)GetDlgItem(IDC_CHINCR3);
		rgu1 = (CButton*)GetDlgItem(IDC_CHSHP);
		rgu2 = (CButton*)GetDlgItem(IDC_CHKO);
		rgu3 = (CButton*)GetDlgItem(IDC_CHCO);
		rgu4 = (CButton*)GetDlgItem(IDC_CHNV);
		rgu5 = (CButton*)GetDlgItem(IDC_56_66);
		rgu6 = (CButton*)GetDlgItem(IDC_CHSINCOS);
		rgu7 = (CButton*)GetDlgItem(IDC_A0);
		CString m_1 = "1", m_0 = "0";
		if (rb1->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rb2->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rb3->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		//RGU-----------------------------------
		if (rgu1->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rgu2->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rgu3->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rgu4->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rgu5->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}

		if (rgu6->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}
		if (rgu7->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}



		CButton* sZDIKM;
		sZDIKM = (CButton*)GetDlgItem(IDC_CHZD_IKM_TSEK);

		if (sZDIKM->GetCheck())
		{

			fputs(m_1 + "\n", foutSV);
		}
		else
		{
			fputs(m_0 + "\n", foutSV);
		}


		fputs(m_sZD + "\n", foutSV);

		fputs(m_sRmax + "\n", foutSV);
		//DR, DN

		fputs(m_sdR1 + "\n", foutSV);
		fputs(m_sdR2 + "\n", foutSV);
		fputs(m_sdN1 + "\n", foutSV);
		fputs(m_sdN2 + "\n", foutSV);
		//---------
		fclose(foutSV);



	}

}

void CFAS_USBDlg::OnChvikl1()
{
	CButton* chV;
	CEdit* edRk;
	chV = (CButton*)GetDlgItem(IDC_CHVIKL1);
	edRk = (CEdit*)GetDlgItem(IDC_EDRK1);
	if (chV->GetCheck())
	{
		edRk->EnableWindow(false);
	}
	else
	{
		edRk->EnableWindow(true);
	}
}

void CFAS_USBDlg::OnChvikl2()
{
	CButton* chV;
	CEdit* edRk;
	chV = (CButton*)GetDlgItem(IDC_CHVIKL2);
	edRk = (CEdit*)GetDlgItem(IDC_EDRK2);
	if (chV->GetCheck())
	{
		edRk->EnableWindow(false);
	}
	else
	{
		edRk->EnableWindow(true);
	}

}

void CFAS_USBDlg::OnChvikl3()
{
	CButton* chV;
	CEdit* edRk;
	chV = (CButton*)GetDlgItem(IDC_CHVIKL3);
	edRk = (CEdit*)GetDlgItem(IDC_EDRK3);
	if (chV->GetCheck())
	{
		edRk->EnableWindow(false);
	}
	else
	{
		edRk->EnableWindow(true);
	}

}

void CFAS_USBDlg::OnChvikl4()
{
	CButton* chV;
	CEdit* edRk;
	chV = (CButton*)GetDlgItem(IDC_CHVIKL4);
	edRk = (CEdit*)GetDlgItem(IDC_EDRK4);
	if (chV->GetCheck())
	{
		edRk->EnableWindow(false);
	}
	else
	{
		edRk->EnableWindow(true);
	}
}

void CFAS_USBDlg::OnCHZDIKMtsek()
{
	CButton* btn;
	btn = (CButton*)GetDlgItem(IDC_CHZD_IKM_TSEK);
	CEdit* pEd;
	pEd = (CEdit*)GetDlgItem(IDC_EDZD);

	m_cZD.GetWindowText(m_sZD);
	float fZD_tek = atof(m_sZD);
	if (btn->GetCheck())
	{
		fZD_tek = fZD_tek * 1000.0 / 15.0;
		m_sZD.Format("%0.3f", fZD_tek);
		UpdateData(false);
		btn->SetWindowText("Сдвиг ИКМ, такты");
	}
	else
	{
		fZD_tek = fZD_tek / 1000.0 * 15.0;
		btn->SetWindowText("Сдвиг ИКМ, мксек");
		m_sZD.Format("%.3f", fZD_tek);
		UpdateData(false);
	}
}

void CFAS_USBDlg::OnChangeEdrmax()
{
	CButton* RMAX;
	RMAX = (CButton*)GetDlgItem(IDC_EDRMAX);
	Get_Rmax();
	tt47.AddTool(RMAX, Rmax_in_device1);

}

void CFAS_USBDlg::OnA0()
{

	CButton* btn;
	CEdit* edNOI;
	btn = (CButton*)GetDlgItem(IDC_A0);
	edNOI = (CEdit*)GetDlgItem(IDC_EDNUMOI);
	if (btn->GetCheck())
	{
		btn->SetWindowText("Альфа 0 Внутр");
		edNOI->EnableWindow(true);
		m_sNumOI = '0';
		edNOI->SetWindowText(m_sNumOI);
	}
	else
	{
		btn->SetWindowText("Альфа 0 Внеш");
		edNOI->EnableWindow(false);
	}
}

void CFAS_USBDlg::OnKillfocusEddrn()
{
	/*
	m_cdRn.GetWindowText(m_sdRn);
	IsCorrect(m_sdRn,0);

		if(bPusk==false)
		{
			if(m_cdRn.GetModify()!=0)
			{
				Dyn_Record(0x1097, m_sdRn,"RG_dRn(10h)",0);

			}
		}	*/
}

void CFAS_USBDlg::OnKillfocusEddrk()
{
	/*m_cdRk.GetWindowText(m_sdRk);
	IsCorrect(m_sdRk,0);

		if(bPusk==false)
		{
			if(m_cdRk.GetModify()!=0)
			{
				Dyn_Record(0x1A97, m_sRn1,"RG_Rn1(1Ah)",0);

			}
		}*/
}

void CFAS_USBDlg::OnKillfocusEddn1()
{
	/*m_cdN1.GetWindowText(m_sdN1);
	IsCorrect(m_sdN1,0);

		if(bPusk==false)
		{
			if(m_cdN1.GetModify()!=0)
			{
				Dyn_Record(0x1E97, m_sdN1,"RG_Rn1(1Eh)",0);

			}
		}
		*/
}

void CFAS_USBDlg::OnKillfocusEddn2()
{
	/*	m_cdN2.GetWindowText(m_sdN2);
	IsCorrect(m_sdN2,0);

		if(bPusk==false)
		{
			if(m_cdN2.GetModify()!=0)
			{
				Dyn_Record(0x1F97, m_sdN2,"RG_dN2(1Fh)",0);

			}
		}
	*/
}
