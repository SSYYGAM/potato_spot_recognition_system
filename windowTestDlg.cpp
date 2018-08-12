
// windowTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windowTest.h"
#include "windowTestDlg.h"
#include "afxdialogex.h"
#include "opencv\cv.h"
#include "opencv\cxcore.h"
#include "opencv\highgui.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


#include <string>
#include <iostream>
using namespace std;
using namespace cv;


extern "C"
{
#include "imgfeatures.h"
#include "kdtree.h"
#include "minpq.h"
#include "sift.h"
#include "utils.h"
#include "xform.h"
}




//��k-d���Ͻ���BBF������������
/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200
#define NN_SQ_DIST_RATIO_THR 0.5

//�������ַ���
#define IMG1 "ͼ1"
#define IMG2 "ͼ2"
#define IMG1_FEAT "ͼ1������"
#define IMG2_FEAT "ͼ2������"
#define IMG_MATCH1 "�����ֵɸѡ���ƥ����"
#define IMG_MATCH2 "RANSACɸѡ���ƥ����"
#define IMG_MOSAIC_TEMP "��ʱƴ��ͼ��"
#define IMG_MOSAIC_SIMPLE "����ƴ��ͼ"
#define IMG_MOSAIC_BEFORE_FUSION "�ص������ں�ǰ"
#define IMG_MOSAIC_PROC12 "ƴ��ͼ1-2"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32771();

	afx_msg void bblocation();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_32771, &CAboutDlg::On32771)
	ON_COMMAND(ID_32786, &CAboutDlg::bblocation)
END_MESSAGE_MAP()


// CwindowTestDlg �Ի���




CwindowTestDlg::CwindowTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwindowTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwindowTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwindowTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CwindowTestDlg::OnBnClickedMfcmenubutton1)
ON_COMMAND(ID_32771, &CwindowTestDlg::On32771)
ON_COMMAND(ID_32774, &CwindowTestDlg::On32774)
ON_COMMAND(ID_32775, &CwindowTestDlg::canny)
ON_COMMAND(ID_Menu, &CwindowTestDlg::LaLace)
ON_COMMAND(ID_32778, &CwindowTestDlg::MaximumConnectedarea)
ON_COMMAND(ID_SIFT32779, &CwindowTestDlg::siftStitching)
ON_UPDATE_COMMAND_UI(ID_32781, &CwindowTestDlg::Fengshuilin)
ON_COMMAND(ID_32782, &CwindowTestDlg::weather)
ON_COMMAND(ID_32784, &CwindowTestDlg::Color)
ON_COMMAND(ID_32785, &CwindowTestDlg::weather2)
ON_COMMAND(ID_32786, &CwindowTestDlg::BBlocation)
ON_COMMAND(ID_32787, &CwindowTestDlg::Median_filter)
ON_COMMAND(ID_HSV_HSV32788, &CwindowTestDlg::hsv_color)
ON_COMMAND(ID_32789, &CwindowTestDlg::lunkuo)
END_MESSAGE_MAP()


// CwindowTestDlg ��Ϣ�������

BOOL CwindowTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwindowTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CwindowTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwindowTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwindowTestDlg::OnBnClickedMfcmenubutton1()
{

}


void CAboutDlg::On32771()
{
	
}

IplImage* SrcImg = NULL;
string mPath_pd ;
Mat src,dst;
Mat bsSrc;
bool flage=true;
int spatialRad=10,colorRad=10,maxPryLevel=1;

//����ͼ2���ĸ��Ǿ�����H�任�������
void CalcFourCorner(CvMat* &H,CvPoint& leftTop,CvPoint& leftBottom, CvPoint& rightTop,CvPoint& rightBottom,IplImage* img2)
{
	//����ͼ2���ĸ��Ǿ�����H�任�������
	double v2[]={0,0,1};//���Ͻ�
	double v1[3];//�任�������ֵ
	CvMat V2 = cvMat(3,1,CV_64FC1,v2);
	CvMat V1 = cvMat(3,1,CV_64FC1,v1);
	cvGEMM(H,&V2,1,0,1,&V1);//����˷�
	leftTop.x = cvRound(v1[0]/v1[2]);
	leftTop.y = cvRound(v1[1]/v1[2]);
	//cvCircle(xformed,leftTop,7,CV_RGB(255,0,0),2);

	//��v2��������Ϊ���½�����
	v2[0] = 0;
	v2[1] = img2->height;
	V2 = cvMat(3,1,CV_64FC1,v2);
	V1 = cvMat(3,1,CV_64FC1,v1);
	cvGEMM(H,&V2,1,0,1,&V1);
	leftBottom.x = cvRound(v1[0]/v1[2]);
	leftBottom.y = cvRound(v1[1]/v1[2]);
	//cvCircle(xformed,leftBottom,7,CV_RGB(255,0,0),2);

	//��v2��������Ϊ���Ͻ�����
	v2[0] = img2->width;
	v2[1] = 0;
	V2 = cvMat(3,1,CV_64FC1,v2);
	V1 = cvMat(3,1,CV_64FC1,v1);
	cvGEMM(H,&V2,1,0,1,&V1);
	rightTop.x = cvRound(v1[0]/v1[2]);
	rightTop.y = cvRound(v1[1]/v1[2]);
	//cvCircle(xformed,rightTop,7,CV_RGB(255,0,0),2);

	//��v2��������Ϊ���½�����
	v2[0] = img2->width;
	v2[1] = img2->height;
	V2 = cvMat(3,1,CV_64FC1,v2);
	V1 = cvMat(3,1,CV_64FC1,v1);
	cvGEMM(H,&V2,1,0,1,&V1);
	rightBottom.x = cvRound(v1[0]/v1[2]);
	rightBottom.y = cvRound(v1[1]/v1[2]);
	//cvCircle(xformed,rightBottom,7,CV_RGB(255,0,0),2);

}

int detectionFeature(IplImage* img,struct feature*& feat)
{
	int n  = sift_features( img, &feat);//���ͼimg�е�SIFT������,n��ͼ�����������
	//export_features("feature.txt",feat,n);//��������������д�뵽�ļ�
	return n;
}

IplImage* spliceImage(IplImage* img1,IplImage* img2)
{
	struct feature *feat1, *feat2;//feat1��ͼ1�����������飬feat2��ͼ2������������
	int n1, n2;//n1:ͼ1�е������������n2��ͼ2�е����������
	struct feature *feat;//ÿ��������
	struct kd_node *kd_root;//k-d��������
	struct feature **nbrs;//��ǰ�����������ڵ�����
	CvMat * H = NULL;//RANSAC�㷨����ı任����
	struct feature **inliers;//��RANSACɸѡ����ڵ�����
	int n_inliers;//��RANSAC�㷨ɸѡ����ڵ����,��feat2�о��з���Ҫ���������ĸ���

	IplImage *xformed = NULL,*xformed_proc = NULL;//xformed��ʱƴ��ͼ����ֻ��ͼ2�任���ͼ,xformed_proc�����պϳɵ�ͼ

	//ͼ2���ĸ��Ǿ�����H�任�������
	CvPoint leftTop,leftBottom,rightTop,rightBottom;
	cvSetImageROI(img1, cvRect(0, 0, 1920, 600));
		IplImage *img11 = cvCreateImage(cvGetSize(img1),img1->depth,img1->nChannels);
		cvCopy(img1, img11, NULL);
		cvSetImageROI(img2, cvRect(0, 0, 1920, 600));
		IplImage *img22 = cvCreateImage(cvGetSize(img2),img1->depth,img1->nChannels);
		cvCopy(img2, img22, NULL);
	///////////////////////////////////////////////////////////////////

	//��������
	n1 = detectionFeature( img11,feat1 );//���ͼ1�е�SIFT������,n1��ͼ1�����������
	//��ȡ����ʾ��2��ͼƬ�ϵ�������
	n2 = detectionFeature( img22, feat2 );//���ͼ2�е�SIFT�����㣬n2��ͼ2�����������
	
	//����ƥ��
	//��ʽһ��ˮƽ����
	//��2��ͼƬ�ϳ�1��ͼƬ,img1����img2����
	//stacked = stack_imgs_horizontal(img1, img2);//�ϳ�ͼ����ʾ�������ֵ��ɸѡ���ƥ����
	//����ͼ1�������㼯feat1����k-d��������k-d������kd_root
	kd_root = kdtree_build( feat1, n1 );
	CvPoint pt1,pt2;//���ߵ������˵�
	double d0,d1;//feat2��ÿ�������㵽����ںʹν��ڵľ���
	int matchNum = 0;//�������ֵ��ɸѡ���ƥ���Եĸ���
	//���������㼯feat2�����feat2��ÿ��������feat��ѡȡ���Ͼ����ֵ������ƥ��㣬�ŵ�feat��fwd_match����
	for(int i = 0; i < n2; i++ )
	{
		feat = feat2+i;//��i���������ָ��
		//��kd_root������Ŀ���feat��2������ڵ㣬�����nbrs�У�����ʵ���ҵ��Ľ��ڵ����
		int k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );//feat������ڵ�ľ����ƽ��
			d1 = descr_dist_sq( feat, nbrs[1] );//feat��ν��ڵ�ľ����ƽ��
			//��d0��d1�ı�ֵС����ֵNN_SQ_DIST_RATIO_THR������ܴ�ƥ�䣬�����޳�
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{   //��Ŀ���feat������ڵ���Ϊƥ����
				pt2.x = cvRound(feat->x);pt2.y = cvRound(feat->y);
				pt1.x = cvRound(nbrs[0]->x); pt1.y = cvRound(nbrs[0]->y);
				pt2.x += img1->width;//��������ͼ���������еģ�pt2�ĺ��������ͼ1�Ŀ�ȣ���Ϊ���ߵ��յ�
				//cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 );//��������
				matchNum++;//ͳ��ƥ���Եĸ���
				feat2[i].fwd_match = nbrs[0];//ʹ��feat��fwd_match��ָ�����Ӧ��ƥ���
			}
		}
		free( nbrs );//�ͷŽ�������
		
	}
	
	//����RANSAC�㷨ɸѡƥ���,����任����H��
	//����img1��img2������˳��H��Զ�ǽ�feat2�е�������任Ϊ��ƥ��㣬����img2�еĵ�任Ϊimg1�еĶ�Ӧ��
	H = ransac_xform(feat2,n2,FEATURE_FWD_MATCH,lsq_homog,4,0.01,homog_xfer_err,3.0,&inliers,&n_inliers);

	//���ܳɹ�������任���󣬼�����ͼ���й�ͬ����
	IplImage* stacked_ransac;
	
	///stacked_ransac = stack_imgs(img1, img2);
	cvResetImageROI(img1);
	cvResetImageROI(img2);
	stacked_ransac = stack_imgs_horizontal(img1, img2);

	if( H )
	{
      int invertNum = 0;//ͳ��pt2.x > pt1.x��ƥ���Եĸ��������ж�img1���Ƿ���ͼ  
  
      //������RANSAC�㷨ɸѡ��������㼯��inliers���ҵ�ÿ���������ƥ��㣬��������  
        for(int i=0; i<n_inliers; i++)  
          {  
             feat = inliers[i];//��i��������  
             pt2 = cvPoint(cvRound(feat->x), cvRound(feat->y));//ͼ2�е������  
             pt1 = cvPoint(cvRound(feat->fwd_match->x), cvRound(feat->fwd_match->y));//ͼ1�е������(feat��ƥ���)  
        
             //ͳ��ƥ��������λ�ù�ϵ�����ж�ͼ1��ͼ2������λ�ù�ϵ  
            if(pt2.x > pt1.x)  
               invertNum++;  
  
			 // pt2.y += img1->height;//��������ͼ���������еģ�pt2�ĺ��������ͼ1�Ŀ�ȣ���Ϊ���ߵ��յ� 
              pt2.x += img1->width;//��������ͼ���������еģ�pt2�ĺ��������ͼ1�Ŀ�ȣ���Ϊ���ߵ��յ�  
              cvLine(stacked_ransac,pt1,pt2,CV_RGB(255,0,255),1,8,0);//��ƥ��ͼ�ϻ�������  
           }  
          cvNamedWindow(IMG_MATCH2,CV_WINDOW_NORMAL);//��������  
          cvShowImage(IMG_MATCH2,stacked_ransac);//��ʾ��RANSAC�㷨ɸѡ���ƥ��ͼ 
		  cvSaveImage("ransac.bmp",stacked_ransac);
		  fprintf( stderr, "Found ransac %d total matches\n", n_inliers );
	 }

	if( H )
	{
		//ȫ��ƴ��
		//���ܳɹ�������任���󣬼�����ͼ���й�ͬ���򣬲ſ��Խ���ȫ��ƴ��
		//ƴ��ͼ��img1����ͼ��img2����ͼ
		CalcFourCorner(H,leftTop,leftBottom,rightTop,rightBottom,img2);//����ͼ2���ĸ��Ǿ��任�������
		//Ϊƴ�ӽ��ͼxformed����ռ�,�߶�Ϊͼ1ͼ2�߶ȵĽ�С�ߣ�����ͼ2���ϽǺ����½Ǳ任��ĵ��λ�þ���ƴ��ͼ�Ŀ��
		xformed = cvCreateImage(cvSize(MIN(rightTop.x,rightBottom.x),MIN(img1->height,img2->height)),IPL_DEPTH_8U,3);
		//�ñ任����H����ͼimg2��ͶӰ�任(�任�������������)������ŵ�xformed��
		cvWarpPerspective(img2,xformed,H,CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,cvScalarAll(0));

		//������ƴ��ͼ����¡��xformed
		xformed_proc = cvCloneImage(xformed);

		//�ص�������ߵĲ�����ȫȡ��ͼ1
		cvSetImageROI(img1,cvRect(0,0,MIN(leftTop.x,leftBottom.x),xformed_proc->height));
		cvSetImageROI(xformed,cvRect(0,0,MIN(leftTop.x,leftBottom.x),xformed_proc->height));
		cvSetImageROI(xformed_proc,cvRect(0,0,MIN(leftTop.x,leftBottom.x),xformed_proc->height));
		cvAddWeighted(img1,1,xformed,0,0,xformed_proc);
		cvResetImageROI(img1);
		cvResetImageROI(xformed);
		cvResetImageROI(xformed_proc);

		////////////////////////////////////////////////////////////
		//ͼ���ں�
		//���ü�Ȩƽ���ķ����ں��ص�����
		int start = MIN(leftTop.x,leftBottom.x) ;//��ʼλ�ã����ص��������߽�
		double processWidth = img1->width - start;//�ص�����Ŀ��
		double alpha = 1;//img1�����ص�Ȩ��
		for(int i=0; i<xformed_proc->height; i++)//������
		{
			const uchar * pixel_img1 = ((uchar *)(img1->imageData + img1->widthStep * i));//img1�е�i�����ݵ�ָ��
			const uchar * pixel_xformed = ((uchar *)(xformed->imageData + xformed->widthStep * i));//xformed�е�i�����ݵ�ָ��
			uchar * pixel_xformed_proc = ((uchar *)(xformed_proc->imageData + xformed_proc->widthStep * i));//xformed_proc�е�i�����ݵ�ָ��
			for(int j=start; j<img1->width; j++)//�����ص��������
			{
				//�������ͼ��xformed�������صĺڵ㣬����ȫ����ͼ1�е�����
				if(pixel_xformed[j*3] < 50 && pixel_xformed[j*3+1] < 50 && pixel_xformed[j*3+2] < 50 )
				{
					alpha = 1;
				}
				else
				{   //img1�����ص�Ȩ�أ��뵱ǰ�������ص�������߽�ľ��������
					alpha = (processWidth-(j-start)) / processWidth ;
				}
				pixel_xformed_proc[j*3] = pixel_img1[j*3] * alpha + pixel_xformed[j*3] * (1-alpha);//Bͨ��
				pixel_xformed_proc[j*3+1] = pixel_img1[j*3+1] * alpha + pixel_xformed[j*3+1] * (1-alpha);//Gͨ��
				pixel_xformed_proc[j*3+2] = pixel_img1[j*3+2] * alpha + pixel_xformed[j*3+2] * (1-alpha);//Rͨ��
			}
		}

	}
	else //�޷�������任���󣬼�����ͼ��û���غ�����
	{
		return NULL;
	}
	///////////////////////////////////////////////////////////////////////////
	kdtree_release(kd_root);//�ͷ�kd��
	//ֻ����RANSAC�㷨�ɹ�����任����ʱ������Ҫ��һ���ͷ�������ڴ�ռ�
	if(H)
	{
		cvReleaseMat(&H);//�ͷű任����H
		free(inliers);//�ͷ��ڵ�����
	}
	if (NULL != xformed)
	{
		cvReleaseImage(&xformed);
	}
	return xformed_proc;
}
void CwindowTestDlg::On32771()
{
	// TODO: �ڴ���������������
    CFileDialog dlg(  
        TRUE, _T("*.bmp;*.jpg;*.tif"), NULL,  
        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,  
        "image files All Files (*.*) |*.*||", NULL  
        );// ѡ��ͼƬ��Լ��;    
    dlg.m_ofn.lpstrTitle = _T("��ͼƬ");// ���ļ��Ի���ı�����;(*.bmp; *.jpg) |*.bmp; *.jpg |    
    if (dlg.DoModal() == IDOK)// �ж��Ƿ���ͼƬ;    
    {  
  
        if (dlg.GetFileExt() != "bmp" && dlg.GetFileExt() != "JPG"&&dlg.GetFileExt() != "tif"&&dlg.GetFileExt() != "jpg")  
        {  
            AfxMessageBox(_T("��ѡ����ȷ��ͼƬ��ʽ��"), MB_OK);  
            return;  
        }  
  
        CString mPath = dlg.GetPathName();// ��ȡͼƬ·��;
		string mpath=dlg.GetPathName();
		mPath_pd=mpath;
  
        SrcImg = cvLoadImage(mPath);//��ȡͼƬ�����浽һ���ֲ�����ipl��;
		bsSrc=imread(mpath, IMREAD_COLOR);
        if (!SrcImg)// �ж��Ƿ�ɹ�����ͼƬ;    
            return;  
    }  

	cvNamedWindow("source1",CV_WINDOW_NORMAL);  
    cvShowImage("source1", SrcImg);
}


void CwindowTestDlg::On32774()
{
	// TODO: �ڴ���������������
	
	cv::Mat matimg;
	Mat dst1;
    matimg = cv::Mat(SrcImg);
	Mat grad_x, grad_y;  
    Mat abs_grad_x, abs_grad_y;  
  
    //��3���� X�����ݶ�  
    Sobel( matimg, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT );  
    convertScaleAbs( grad_x, abs_grad_x );    
  
    //��4����Y�����ݶ�  
    Sobel( matimg, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT );  
    convertScaleAbs( grad_y, abs_grad_y );     
  
    //��5���ϲ��ݶ�(����)  
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst1 ); 
	namedWindow("Sobel", CV_WINDOW_NORMAL);
    imshow("Sobel", dst1);   
    waitKey(0);   
       
}


void CwindowTestDlg::canny()
{
	// TODO: �ڴ���������������
	cv::Mat matimg;
    matimg = cv::Mat(SrcImg);
	Mat dst1,edge,gray; 
	Mat src1=matimg.clone();
  
    // ��1��������srcͬ���ͺʹ�С�ľ���(dst)  
    dst1.create( src1.size(), src1.type() );  
  
    // ��2����ԭͼ��ת��Ϊ�Ҷ�ͼ��  
    cvtColor( src1, gray, CV_BGR2GRAY );  
  
    // ��3������ʹ�� 3x3�ں�������  
    blur( gray, edge, Size(3,3) );  
  
    // ��4������Canny����  
    Canny( edge, edge, 3,9,3 );  
  
    //��5����g_dstImage�ڵ�����Ԫ������Ϊ0   
    dst1 = Scalar::all(0);  
  
    //��6��ʹ��Canny��������ı�Եͼg_cannyDetectedEdges��Ϊ���룬����ԭͼg_srcImage����Ŀ��ͼg_dstImage��  
   // src1.copyTo( dst, edge);  
  
    //��7����ʾЧ��ͼ  
	namedWindow("Canny", CV_WINDOW_NORMAL);
    imshow("Canny", edge);   
    waitKey(0);   
}


void CwindowTestDlg::LaLace()
{
	// TODO: �ڴ���������������
	Mat src_gray,dst1, abs_dst;  
    cv::Mat matimg;
    matimg = cv::Mat(SrcImg);
  
    //��3��ʹ�ø�˹�˲���������  
    GaussianBlur( matimg, matimg, Size(3,3), 0, 0, BORDER_DEFAULT );  
  
    //��4��ת��Ϊ�Ҷ�ͼ  
    cvtColor( matimg, src_gray, CV_RGB2GRAY );  
  
    //��5��ʹ��Laplace����  
    Laplacian( src_gray, dst1, CV_16S, 3, 1, 0, BORDER_DEFAULT );  
  
    //��6���������ֵ���������ת����8λ  
    convertScaleAbs( dst1, abs_dst );  
  
    //��7����ʾЧ��ͼ 
	namedWindow("Laplace", CV_WINDOW_NORMAL);
    imshow( "Laplace", abs_dst );  
  
    waitKey(0);   
}


void CwindowTestDlg::MaximumConnectedarea()
{
	// TODO: �ڴ���������������
	IplImage* src1 = cvCreateImage(cvGetSize(SrcImg), IPL_DEPTH_8U, 1);
	cvCvtColor(SrcImg, src1, CV_RGB2GRAY);
	IplImage* src2 = SrcImg; 
    IplImage* dst1 = cvCreateImage(cvGetSize(src1), 8, 3);  
    CvMemStorage* storage = cvCreateMemStorage(0);  
    CvSeq* contour = 0;  
    cvThreshold(src1, src1,120, 255, CV_THRESH_BINARY);   // ��ֵ��  
    cvNamedWindow("Source",CV_WINDOW_NORMAL);  
    cvShowImage("Source", src1);  
    // ��ȡ����  
    int contour_num = cvFindContours(src1, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);  
    cvZero(dst1);        // �������  
    CvSeq *_contour = contour;   
    double maxarea = 0;  
    double minarea = 100;  
    int m = 0;  
    for( ; contour != 0; contour = contour->h_next )    
    {    
  
        double tmparea = fabs(cvContourArea(contour));  
        if(tmparea < minarea)     
        {    
            cvSeqRemove(contour, 0); // ɾ�����С���趨ֵ������  
            continue;  
        }    
        CvRect aRect = cvBoundingRect( contour, 0 );   
        if ((aRect.width/aRect.height)<1)    
        {    
            cvSeqRemove(contour, 0); //ɾ����߱���С���趨ֵ������  
            continue;  
        }    
        if(tmparea > maxarea)    
        {    
            maxarea = tmparea;  
        }    
        m++;  
        // ����һ��ɫ��ֵ  
        CvScalar color = CV_RGB( 0, 255, 255 );   
        cvDrawContours(dst1, contour, color, color, -1, 1, 8);   //�����ⲿ���ڲ������� 
		cvDrawContours(src2, contour, color, color, -1, 1, 8);
    }    
    contour = _contour;  
    int count = 0;  
    for(; contour != 0; contour = contour->h_next)  
    {    
        count++;  
        double tmparea = fabs(cvContourArea(contour));  
        if (tmparea == maxarea)    
        {    
            CvScalar color = CV_RGB( 255, 0, 0);  
            cvDrawContours(dst1, contour, color, color, -1, 1, 8);
			cvDrawContours(src2, contour, color, color, -1, 1, 8);
        }    
    }    
    printf("The total number of contours is:%d", count);  
    cvNamedWindow("Components", CV_WINDOW_NORMAL);  
    cvShowImage("Components", dst1);
	cvNamedWindow("source2",CV_WINDOW_NORMAL);  
    cvShowImage("source2", src2);
    cvWaitKey(0);  
    cvDestroyWindow("Source");  
    cvReleaseImage(&src1);  
    cvDestroyWindow("Components");  
    cvReleaseImage(&dst1); 
}


void CwindowTestDlg::siftStitching()
{
	// TODO: �ڴ���������������
	string name,name1,name2,name3,name4,name5,name6,name7,name8,name9,name10,name11,name12,name13,name14,name15,name16;
	IplImage *img1=NULL, *img2=NULL;/*,*img3=NULL, *img4=NULL,*img5=NULL, *img6=NULL,*img7=NULL, *img8=NULL,*img9=NULL, *img14=NULL,
		*img10=NULL, *img11=NULL,*img12=NULL, *img13=NULL,*img15=NULL, *img16=NULL;*/
	IplImage* imgtotal= NULL;

	int n1,n2;
	struct feature* feat1, * feat2,* feat3,* feat4,* feat5,* feat6,* feat7,* feat8,* feat9,* feat10,* feat11,* feat12,* feat13,* feat14,* feat15,* feat16, * feat; 
	IplImage *img1_Feat=NULL, *img2_Feat=NULL,*img3_Feat=NULL,*img4_Feat=NULL,*img5_Feat=NULL,*img6_Feat=NULL,*img7_Feat=NULL,*img8_Feat=NULL,*img9_Feat=NULL,
		*img10_Feat=NULL,*img11_Feat=NULL,*img12_Feat=NULL,*img13_Feat=NULL,*img14_Feat=NULL,*img15_Feat=NULL,*img16_Feat=NULL;
	
	IplImage* stacked;IplImage* stacked_ransac;
	struct feature** nbrs; 
	struct kd_node* kd_root; 

	///////////////////////////////////////////////////

	//����ͼƬ
	 TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg)|*.jpg|bmp�ļ�(*.bmp)|*.bmp||");   
    //�ļ�����˵������չ������ | �ָ���ÿ���ļ����ͼ��� | �ָ���ĩβ�� || ָ����
    CFileDialog fileDlg(TRUE, _T("*.bmp;*.jpg;*.tif"),NULL,OFN_ALLOWMULTISELECT,"image files All Files (*.*) |*.*||",this);
    if(fileDlg.DoModal() != IDOK)     //û�е�ȷ����ť
        return;
    POSITION pos = fileDlg.GetStartPosition();
   
        
        CString szPathName = fileDlg.GetNextPathName(pos);  
    //CString CFileDialog::GetNextPathName( POSITION& pos ) ����ѡ���˶���ļ�������õ���һ���ļ�λ�ã���ͬʱ���ص�ǰ�ļ������������Ѿ����ù�POSITION CFileDialog::GetStartPosition( )���õ������POSITION������
        TRACE( _T("%s/n"), szPathName);    
        img1 = cvLoadImage(szPathName);
		 szPathName = fileDlg.GetNextPathName(pos);  
    //CString CFileDialog::GetNextPathName( POSITION& pos ) ����ѡ���˶���ļ�������õ���һ���ļ�λ�ã���ͬʱ���ص�ǰ�ļ������������Ѿ����ù�POSITION CFileDialog::GetStartPosition( )���õ������POSITION������
        TRACE( _T("%s/n"), szPathName);    
        img2 = cvLoadImage(szPathName);
       // imgs.push_back(imgg);
	
	
		//img1 = cvLoadImage( name1.c_str());//��ͼ1��ǿ�ƶ�ȡΪ��ͨ��ͼ��
		cvSetImageROI(img1, cvRect(0, 0, 1920, 600));
		IplImage *img11 = cvCreateImage(cvGetSize(img1),img1->depth,img1->nChannels);
		cvCopy(img1, img11, NULL);

		//img2 = cvLoadImage(name2.c_str());
		cvSetImageROI(img2, cvRect(0, 0, 1920, 600));
		IplImage *img22 = cvCreateImage(cvGetSize(img2),img2->depth,img2->nChannels);
		cvCopy(img2, img22, NULL);
		

	//sift������ȡ
	cvResetImageROI(img1);
	cvResetImageROI(img2);
	img1_Feat = cvCloneImage(img1);//����ͼ1�������������������
    img2_Feat = cvCloneImage(img2);//����ͼ2�������������������
    //Ĭ����ȡ����LOWE��ʽ��SIFT������
    //��ȡ����ʾ��1��ͼƬ�ϵ�������
    n1 = sift_features( img11, &feat1 );//���ͼ1�е�SIFT������,n1��ͼ1�����������
    export_features("featureb1.txt",feat1,n1);//��������������д�뵽�ļ�
    draw_features( img1_Feat, feat1, n1 );//����������
	/*img1 = cvCloneImage(img11);
	cvResetImageROI(img1);*/
    cvNamedWindow(IMG1_FEAT,CV_WINDOW_NORMAL);//��������
    cvShowImage(IMG1_FEAT,img1_Feat);//��ʾ
	cvSaveImage("1_feature.bmp",img1_Feat);

    //��ȡ����ʾ��2��ͼƬ�ϵ�������
    n2 = sift_features( img22, &feat2 );//���ͼ2�е�SIFT�����㣬n2��ͼ2�����������
    export_features("featureb2.txt",feat2,n2);//��������������д�뵽�ļ�
    draw_features( img2_Feat, feat2, n2 );//����������
	/*img2 = cvCloneImage(img22);
	cvResetImageROI(img2);*/
    cvNamedWindow(IMG2_FEAT,CV_WINDOW_NORMAL);//��������
    cvShowImage(IMG2_FEAT,img2_Feat);//��ʾ
	cvSaveImage("2_feature.bmp",img2_Feat);
	/*cvResetImageROI(img1);
	cvResetImageROI(img2);*/

	//����ͼ1�������㼯feat1����k-d��������k-d������kd_root
    kd_root = kdtree_build( feat1, n1 );

    CvPoint pt1,pt2;//���ߵ������˵�
    double d0,d1;//feat2��ÿ�������㵽����ںʹν��ڵľ���
    int matchNum = 0;//�������ֵ��ɸѡ���ƥ���Եĸ���

	//stacked = stack_imgs(img1, img2);
	
	stacked = stack_imgs_horizontal(img1, img2);

    //���������㼯feat2�����feat2��ÿ��������feat��ѡȡ���Ͼ����ֵ������ƥ��㣬�ŵ�feat��fwd_match����
    for(int i = 0; i < n2; i++ )
    {
        feat = feat2+i;//��i���������ָ��
        //��kd_root������Ŀ���feat��2������ڵ㣬�����nbrs�У�����ʵ���ҵ��Ľ��ڵ����
        int k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
        if( k == 2 )
        {
            d0 = descr_dist_sq( feat, nbrs[0] );//feat������ڵ�ľ����ƽ��
            d1 = descr_dist_sq( feat, nbrs[1] );//feat��ν��ڵ�ľ����ƽ��
            //��d0��d1�ı�ֵС����ֵNN_SQ_DIST_RATIO_THR������ܴ�ƥ�䣬�����޳�
            if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
            {   //��Ŀ���feat������ڵ���Ϊƥ����
                pt2 = cvPoint( cvRound( feat->x ), cvRound( feat->y ) );//ͼ2�е������
                pt1 = cvPoint( cvRound( nbrs[0]->x ), cvRound( nbrs[0]->y ) );//ͼ1�е������(feat������ڵ�)

                pt2.x += img1->width;//��������ͼ���������еģ�pt2�ĺ��������ͼ1�Ŀ�ȣ���Ϊ���ߵ��յ�
				//pt2.y += img1->height;//��������ͼ���������еģ�pt2�ĺ��������ͼ1�Ŀ�ȣ���Ϊ���ߵ��յ�

                cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 );//��������
                matchNum++;//ͳ��ƥ���Եĸ���
                feat2[i].fwd_match = nbrs[0];//ʹ��feat��fwd_match��ָ�����Ӧ��ƥ���
            }
        }
        free( nbrs );//�ͷŽ�������
    }
    fprintf( stderr, "Found %d total matches\n", matchNum ); 
    //��ʾ�����澭�����ֵ��ɸѡ���ƥ��ͼ
    cvNamedWindow(IMG_MATCH1,CV_WINDOW_NORMAL);//��������
    cvShowImage(IMG_MATCH1,stacked);//��ʾ
	cvSaveImage("pipei.bmp",stacked);
	////////////////////////////////////////////////////////

	//�˴�Ӧͳһ���������㣬����ƥ�䣬Ȼ��ͳһ����ƴ�ӣ�ֱ��ƴ�ӳ���ͼ

	///////////////////////////////////////////////////////

	imgtotal = spliceImage(img1,img2);
	

	////////////////////////////////////////////
	if (NULL != imgtotal)
	{
		cvNamedWindow("ƴ�Ӻ�", CV_WINDOW_NORMAL);//��������
		cvShowImage("ƴ�Ӻ�",imgtotal);//��ʾ������ƴ��ͼ
		cvSaveImage("gg12.bmp",imgtotal);
		cvWaitKey(10);
	}
	
	//////////////////////////////////////////////////////////////////////////////

	cvWaitKey(0);

	if(NULL != img1)
	{
		cvReleaseImage(&img1);
	}

	if(NULL != img2)
	{
		cvReleaseImage(&img2);
	}
	
	if (NULL != imgtotal)
	{
		cvReleaseImage(&imgtotal);
		cvDestroyWindow(IMG_MOSAIC_PROC12);//��ʾ������ƴ��ͼ
	}
}


Vec3b RandomColor(int value)    //<span style="line-height: 20.8px; font-family: sans-serif;">//���������ɫ����</span>  
{  
    value=value%255;  //����0~255�������  
    RNG rng;  
    int aa=rng.uniform(0,value);  
    int bb=rng.uniform(0,value);  
    int cc=rng.uniform(0,value);  
    return Vec3b(aa,bb,cc);  
}   
void CwindowTestDlg::Fengshuilin(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	
	 Mat image= cv::Mat(SrcImg);    //����RGB��ɫͼ��  
	 namedWindow("Source Image", CV_WINDOW_NORMAL);
    imshow("Source Image",image);  
  
    //�ҶȻ����˲���Canny��Ե���  
    Mat imageGray;  
    cvtColor(image,imageGray,CV_RGB2GRAY);//�Ҷ�ת��  
    GaussianBlur(imageGray,imageGray,Size(5,5),2);   //��˹�˲�      
    Canny(imageGray,imageGray,80,150);    
 
  
    //��������  
    vector<vector<Point>> contours;    
    vector<Vec4i> hierarchy;    
    findContours(imageGray,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point());    
    Mat imageContours=Mat::zeros(image.size(),CV_8UC1);  //����     
    Mat marks(image.size(),CV_32S);   //Opencv��ˮ��ڶ����������  
    marks=Scalar::all(0);  
    int index = 0;  
    int compCount = 0;  
    for( ; index >= 0; index = hierarchy[index][0], compCount++ )   
    {  
        //��marks���б�ǣ��Բ�ͬ������������б�ţ��൱������עˮ�㣬�ж������������ж���עˮ��  
        drawContours(marks, contours, index, Scalar::all(compCount+1), 1, 8, hierarchy);  
        drawContours(imageContours,contours,index,Scalar(255),1,8,hierarchy);    
    }  
  
    //��������һ�´���ľ���marks����ʲô����  
    Mat marksShows;  
    convertScaleAbs(marks,marksShows); 
	namedWindow("marksShow", CV_WINDOW_NORMAL);
	namedWindow("����", CV_WINDOW_NORMAL);
    imshow("marksShow",marksShows);  
    imshow("����",imageContours);  
    watershed(image,marks);  
  
    //����������һ�·�ˮ���㷨֮��ľ���marks����ʲô����  
    Mat afterWatershed;  
    convertScaleAbs(marks,afterWatershed);
	namedWindow("After Watershed", CV_WINDOW_NORMAL); 
    imshow("After Watershed",afterWatershed);  
  
    //��ÿһ�����������ɫ���  
    Mat PerspectiveImage=Mat::zeros(image.size(),CV_8UC3);  
    for(int i=0;i<marks.rows;i++)  
    {  
        for(int j=0;j<marks.cols;j++)  
        {  
            int index=marks.at<int>(i,j);  
            if(marks.at<int>(i,j)==-1)  
            {  
                PerspectiveImage.at<Vec3b>(i,j)=Vec3b(255,255,255);  
            }              
            else  
            {  
                PerspectiveImage.at<Vec3b>(i,j) =RandomColor(index);  
            }  
        }  
    }  
	namedWindow("After ColorFill", CV_WINDOW_NORMAL); 
    imshow("After ColorFill",PerspectiveImage);  
  
    //�ָ�����ɫ�Ľ����ԭʼͼ���ں�  
    Mat wshed;  
    addWeighted(image,0.4,PerspectiveImage,0.6,0,wshed);  
	namedWindow("AddWeighted Image", CV_WINDOW_NORMAL); 
    imshow("AddWeighted Image",wshed);  
  
    waitKey();  
}


void CwindowTestDlg::weather()
{
	

}

int BinarizeImageByOTSU (IplImage * src)  
{   
    assert(src != NULL);  
     
    //get the ROI   
    CvRect rect = cvGetImageROI(src);  
     
    //information of the source image   
    int x = rect.x;  
    int y = rect.y;  
    int width = rect.width;   
    int height = rect.height;  
    int ws = src->widthStep;  
     
    int thresholdValue=1;//��ֵ   
    int ihist [256] ; // ͼ��ֱ��ͼ, 256����   
    int i, j, k,n, n1, n2, Color=0;  
    double m1, m2, sum, csum, fmax, sb;  
    memset (ihist, 0, sizeof (ihist)) ; // ��ֱ��ͼ�� ��...   
     
    for (i=y;i< y+height;i++) // ����ֱ��ͼ   
    {   
        int mul =  i*ws;  
        for (j=x;j<x+width;j++)  
        {   
            //Color=Point (i,j) ;   
            Color = (int)(unsigned char)*(src->imageData + mul+ j);  
            ihist [Color] +=1;  
        }  
    }  
    sum=csum=0.0;  
    n=0;  
    for (k = 0; k <= 255; k++)  
    {   
        sum+= (double) k* (double) ihist [k] ; // x*f (x) ������   
        n +=ihist [k]; //f (x) ����   
    }  
    // do the otsu global thresholding method   
    fmax = - 1.0;  
    n1 = 0;  
    for (k=0;k<255;k++)   
    {  
        n1+=ihist [k] ;  
        if (! n1)  
        {   
            continue;   
        }  
        n2=n- n1;  
        if (n2==0)   
        {  
            break;  
        }  
        csum+= (double) k*ihist [k] ;  
        m1=csum/ n1;  
        m2= (sum- csum) /n2;  
        sb = ( double) n1* ( double) n2* ( m1 - m2) * (m1- m2) ;  
         
        if (sb>fmax)   
        {  
            fmax=sb;  
            thresholdValue=k;  
        }  
    }  
     
    //binarize the image    
    cvThreshold( src, src ,thresholdValue, 255, CV_THRESH_BINARY );   
    return 0;  
}  

void CwindowTestDlg::Color()
{
	// TODO: �ڴ���������������
	IplImage* img0; 
    IplImage* img;
    IplImage* src;
    IplImage* dst_gray;
	img0=SrcImg;
    if(img0)//����ͼ��   
    {  
		
		//cvSmooth(img0,smooth,CV_GAUSSIAN,3,3);
        img = cvCreateImage( cvSize(img0->width,img0->height), 8, 3);
        cvResize(img0,img);
        src = cvCreateImage( cvSize(img0->width,img0->height), 8, 3);
        cvResize(img0,src);
		//cvSetImageROI(src, cvRect(0, 0, 1920, 600));
		cvSetImageROI(src, cvRect(0, 600, 2000, 3000));
		cvSmooth(src,src,CV_GAUSSIAN,17,17);
		cvResetImageROI(src);
		cvErode(src,src);
		cvErode(src,src);
		cvErode(src,src);
		//cvErode(src,src);
		cvNamedWindow("src", CV_WINDOW_NORMAL);  
        cvShowImage( "src", src);
        cvNamedWindow("img0", CV_WINDOW_NORMAL);
        cvNamedWindow( "img0", 1 );  
        cvShowImage( "img0", img );
        //Ϊ������ʾͼ������ռ�,3ͨ��ͼ���Ա��ò�ɫ��ʾ   
        IplImage* dst = cvCreateImage( cvGetSize(src), 8, 3);   
        CvMemStorage* storage = cvCreateMemStorage(0);  
         
        CvScalar s;
        for(int i = 0;i < src->height;i++)
        {
            for(int j = 0;j < src->width;j++)
            {
                 
                s = cvGet2D(src,i,j); // get the (i,j) pixel value
                if(s.val[0]<240&&s.val[1]<255&&s.val[2]>120)
                {
                    s.val[0]=235;
                    s.val[1]=206;
                    s.val[2]=135;
                }
				else if(s.val[0]<10&&s.val[1]<240&&s.val[2]<30)
                {
                    s.val[0]=0;
                    s.val[1]=0;
                    s.val[2]=0;
                }
				else if(s.val[0]<10&&s.val[1]<240&&s.val[2]<800&&s.val[2]>30)
                {
                    s.val[0]=0;
                    s.val[1]=252;
                    s.val[2]=124;
                }
                else
                {
                    s.val[0]=0;
                    s.val[1]=0;
                    s.val[2]=0;
                }
 
                cvSet2D(src,i,j,s);   //��������
            }
        }
		cvSetImageROI(src, cvRect(0, 300, 1000, 100));
		cvErode(src,src);
		cvResetImageROI(src);
		cvSetImageROI(src, cvRect(0, 500, 2000, 100));
		cvErode(src,src);
		cvErode(src,src);
		cvErode(src,src);
		cvErode(src,src);
		cvErode(src,src);
		cvErode(src,src);
		cvResetImageROI(src);
		cvSetImageROI(src, cvRect(0, 600, 2000, 3000));
		cvDilate(src,src);
		cvDilate(src,src);
		cvDilate(src,src);
		cvDilate(src,src);
		cvResetImageROI(src);
        cvNamedWindow( "image", CV_WINDOW_NORMAL ); 
        cvShowImage("image",src);
 
        dst_gray=cvCreateImage(cvGetSize(src),8,1);
 
        cvCvtColor(src,dst_gray,CV_BGR2GRAY);
        cvNamedWindow( "�Ҷ�ͼ",CV_WINDOW_NORMAL ); 
        cvShowImage("�Ҷ�ͼ",dst_gray);
 
 
        //�ɶ�̬����Ԫ������   
        CvSeq* contour = 0;  
        //��ͼ��������ʶ�ֵ��   
        BinarizeImageByOTSU(dst_gray);  
		cvNamedWindow("Source", CV_WINDOW_NORMAL);
        cvNamedWindow( "Source", 1 );  
        cvShowImage( "Source", dst_gray );  
        //�ڶ�ֵͼ����Ѱ������   
        cvFindContours( dst_gray, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );  
 
 
        cvZero( dst );//�������   
        cvCvtColor(dst_gray,dst,CV_GRAY2BGR);  
        //Ŀ��������С����   
        int mix_area = 25000;  
        //Ŀ�������������   
        int max_area = 350000;  
        //�ɴ����1-��2-��3-��4-TUPLE���͵��������ݵ�����   
        CvScalar color = CV_RGB( 255, 0, 0);  
        //��ͼ���л����ⲿ���ڲ�������   
        for( ; contour != 0; contour = contour->h_next)  
        {  
            //ȡ����������С����   
            CvRect aRect = cvBoundingRect( contour, 1 );  
            //ȡ�þ��ε����   
            int tmparea=aRect.height*aRect.height;    
            if (((double)aRect.width/(double)aRect.height>1.2)  
                && ((double)aRect.width/(double)aRect.height<2.65)&&((double)aRect.width>30)&&((double)aRect.width<133))  
            {  
                cvRectangle(img,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width ,aRect.y+aRect.height),color,2);  
            }  
        }  
        //cvNamedWindow( "img", CV_WINDOW_NORMAL );  
        //cvShowImage( "img", img );
        cvWaitKey(0); 
        cvDestroyWindow("img0");
        cvDestroyWindow("img");
        cvDestroyWindow("�Ҷ�ͼ");
        cvReleaseImage(&dst);  
        cvDestroyWindow("Source");  
        cvReleaseImage(&dst_gray);
        cvReleaseImage(&src);
        cvReleaseImage(&img);
        cvReleaseImage(&img0);
         
       
    }     
   

}


void CwindowTestDlg::weather2()
{
	
	MessageBox(_T("���߲�")); 
	
	
	/*Mat a=cv::Mat(SrcImg);
	Mat d;
    flip(a,d,0);

	
	Mat b=d(Range(1,235), Range(1,a.cols));
	Mat c;
	int num=0 ;
	cvtColor(b,c,CV_BGR2GRAY);
	for(int i=0;i<c.rows;i++)
	{
		for(int j=0;j<c.cols;j++)
		    {
				if(c.at<uchar>(i,j)>=240)
                     num++;
		     }
	}
	
	if (num<500)
	{  
		MessageBox(_T("���죡"));  
	} 
	else
	{
	  MessageBox(_T("���ƣ�"));
	}*/
}





 

 











static void help()  
{  
    cout << "\nThis program demonstrates GrabCut segmentation -- select an object in a region\n"  
        "and then grabcut will attempt to segment it out.\n"  
        "Call:\n"  
        "./grabcut <image_name>\n"  
        "\nSelect a rectangular area around the object you want to segment\n" <<  
        "\nHot keys: \n"  
        "\tESC - quit the program\n"  
        "\tr - restore the original image\n"  
        "\tn - next iteration\n"  
        "\n"  
        "\tleft mouse button - set rectangle\n"  
        "\n"  
        "\tCTRL+left mouse button - set GC_BGD pixels\n"  
        "\tSHIFT+left mouse button - set CG_FGD pixels\n"  
        "\n"  
        "\tCTRL+right mouse button - set GC_PR_BGD pixels\n"  
        "\tSHIFT+right mouse button - set CG_PR_FGD pixels\n" << endl;  
}  
  
const Scalar RED = Scalar(0,0,255);  
const Scalar PINK = Scalar(230,130,255);  
const Scalar BLUE = Scalar(255,0,0);  
const Scalar LIGHTBLUE = Scalar(255,255,160);  
const Scalar GREEN = Scalar(0,255,0);  
  
const int BGD_KEY = CV_EVENT_FLAG_CTRLKEY;  //Ctrl��  
const int FGD_KEY = CV_EVENT_FLAG_SHIFTKEY; //Shift��  
  
static void getBinMask( const Mat& comMask, Mat& binMask )  
{  
    if( comMask.empty() || comMask.type()!=CV_8UC1 )  
        CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );  
    if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols )  
        binMask.create( comMask.size(), CV_8UC1 );  
    binMask = comMask & 1;  //�õ�mask�����λ,ʵ������ֻ����ȷ���Ļ����п��ܵ�ǰ���㵱��mask  
}  
class GCApplication  
{  
public:  
	enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };  
    static const int radius = 2;  
    static const int thickness = -1;  
  
    void reset();  
    void setImageAndWinName( const Mat& _image, const string& _winName );  
    void showImage() const;  
    void mouseClick( int event, int x, int y, int flags, void* param );  
    int nextIter();  
    int getIterCount() const { return iterCount; }  
private:  
    void setRectInMask();  
    void setLblsInMask( int flags, Point p, bool isPr );  
  
    const string* winName;  
    const Mat* image;  
    Mat mask;  
    Mat bgdModel, fgdModel;  
  
    uchar rectState, lblsState, prLblsState;  
    bool isInitialized;  
  
    Rect rect;  
    vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;  
    int iterCount;  
}; 
void GCApplication::reset()  
{  
    if( !mask.empty() )  
        mask.setTo(Scalar::all(GC_BGD));  
    bgdPxls.clear(); fgdPxls.clear();  
    prBgdPxls.clear();  prFgdPxls.clear();  
  
    isInitialized = false;  
    rectState = NOT_SET;    //NOT_SET == 0  
    lblsState = NOT_SET;  
    prLblsState = NOT_SET;  
    iterCount = 0;  
}  
  
/*����ĳ�Ա������ֵ����*/  
void GCApplication::setImageAndWinName( const Mat& _image, const string& _winName  )  
{  
    if( _image.empty() || _winName.empty() )  
        return;  
    image = &_image;  
    winName = &_winName;  
    mask.create( image->size(), CV_8UC1);  
    reset();  
}  
  
/*��ʾ4���㣬һ�����κ�ͼ�����ݣ���Ϊ����Ĳ���ܶ�ط���Ҫ�õ�������������Ե����ó���*/  
void GCApplication::showImage() const  
{  
    if( image->empty() || winName->empty() )  
        return;  
  
    Mat res;  
    Mat binMask;  
    if( !isInitialized )  
        image->copyTo( res );  
    else  
    {  
        getBinMask( mask, binMask );  
        image->copyTo( res, binMask );  //�������λ��0����1�����ƣ�ֻ������ǰ���йص�ͼ�񣬱���˵���ܵ�ǰ�������ܵı���  
    }  
  
    vector<Point>::const_iterator it;  
    /*����4������ǽ�ѡ�е�4�����ò�ͬ����ɫ��ʾ����*/  
    for( it = bgdPxls.begin(); it != bgdPxls.end(); ++it )  //���������Կ�����һ��ָ��  
        circle( res, *it, radius, BLUE, thickness );  
    for( it = fgdPxls.begin(); it != fgdPxls.end(); ++it )  //ȷ����ǰ���ú�ɫ��ʾ  
        circle( res, *it, radius, RED, thickness );  
    for( it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it )  
        circle( res, *it, radius, LIGHTBLUE, thickness );  
    for( it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it )  
        circle( res, *it, radius, PINK, thickness );  
  imwrite("Grabcut.jpg",res);
  imshow("Grabcut",res);
    /*������*/  
    if( rectState == IN_PROCESS || rectState == SET )  
        rectangle( res, Point( rect.x, rect.y ), Point(rect.x + rect.width, rect.y + rect.height ), GREEN, 2);  
  
    imshow( *winName, res ); 
	
}  
  
/*�ò�����ɺ�maskͼ����rect�ڲ���3������ȫ��0*/  
void GCApplication::setRectInMask()  
{  
    assert( !mask.empty() );  
    mask.setTo( GC_BGD );   //GC_BGD == 0  
    rect.x = max(0, rect.x);  
    rect.y = max(0, rect.y);  
    rect.width = min(rect.width, image->cols-rect.x);  
    rect.height = min(rect.height, image->rows-rect.y);  
    (mask(rect)).setTo( Scalar(GC_PR_FGD) );    //GC_PR_FGD == 3�������ڲ�,Ϊ���ܵ�ǰ����  
}  
  
void GCApplication::setLblsInMask( int flags, Point p, bool isPr )  
{  
    vector<Point> *bpxls, *fpxls;  
    uchar bvalue, fvalue;  
    if( !isPr ) //ȷ���ĵ�  
    {  
        bpxls = &bgdPxls;  
        fpxls = &fgdPxls;  
        bvalue = GC_BGD;    //0  
        fvalue = GC_FGD;    //1  
    }  
    else    //���ʵ�  
    {  
        bpxls = &prBgdPxls;  
        fpxls = &prFgdPxls;  
        bvalue = GC_PR_BGD; //2  
        fvalue = GC_PR_FGD; //3  
    }  
    if( flags & BGD_KEY )  
    {  
        bpxls->push_back(p);  
        circle( mask, p, radius, bvalue, thickness );   //�õ㴦Ϊ2  
    }  
    if( flags & FGD_KEY )  
    {  
        fpxls->push_back(p);  
        circle( mask, p, radius, fvalue, thickness );   //�õ㴦Ϊ3  
    }  
}  
  
/*�����Ӧ����������flagsΪCV_EVENT_FLAG�����*/  
void GCApplication::mouseClick( int event, int x, int y, int flags, void* )  
{  
    // TODO add bad args check  
    switch( event )  
    {  
    case CV_EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels  
        {  
            bool isb = (flags & BGD_KEY) != 0,  
                isf = (flags & FGD_KEY) != 0;  
            if( rectState == NOT_SET && !isb && !isf )//ֻ���������ʱ  
            {  
                rectState = IN_PROCESS; //��ʾ���ڻ�����  
                rect = Rect( x, y, 1, 1 );  
            }  
            if ( (isb || isf) && rectState == SET ) //������alt������shift�����һ����˾��Σ���ʾ���ڻ�ǰ��������  
                lblsState = IN_PROCESS;  
        }  
        break;  
    case CV_EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels  
        {  
            bool isb = (flags & BGD_KEY) != 0,  
                isf = (flags & FGD_KEY) != 0;  
            if ( (isb || isf) && rectState == SET ) //���ڻ����ܵ�ǰ��������  
                prLblsState = IN_PROCESS;  
        }  
        break;  
    case CV_EVENT_LBUTTONUP:  
        if( rectState == IN_PROCESS )  
        {  
            rect = Rect( Point(rect.x, rect.y), Point(x,y) );   //���ν���  
            rectState = SET;  
            setRectInMask();  
            assert( bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty() );  
            showImage();  
        }  
        if( lblsState == IN_PROCESS )   //�ѻ���ǰ�󾰵�  
        {  
            setLblsInMask(flags, Point(x,y), false);    //����ǰ����  
            lblsState = SET;  
            showImage();  
        }  
        break;  
    case CV_EVENT_RBUTTONUP:  
        if( prLblsState == IN_PROCESS )  
        {  
            setLblsInMask(flags, Point(x,y), true); //����������  
            prLblsState = SET;  
            showImage();  
        }  
        break;  
    case CV_EVENT_MOUSEMOVE:  
        if( rectState == IN_PROCESS )  
        {  
            rect = Rect( Point(rect.x, rect.y), Point(x,y) );  
            assert( bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty() );  
            showImage();    //���ϵ���ʾͼƬ  
        }  
        else if( lblsState == IN_PROCESS )  
        {  
            setLblsInMask(flags, Point(x,y), false);  
            showImage();  
        }  
        else if( prLblsState == IN_PROCESS )  
        {  
            setLblsInMask(flags, Point(x,y), true);  
            showImage();  
        }  
        break;  
    }  
}  
  
/*�ú�������grabcut�㷨�����ҷ����㷨���е����Ĵ���*/  
int GCApplication::nextIter()  
{  
    if( isInitialized )  
        //ʹ��grab�㷨����һ�ε���������2Ϊmask��������maskλ�ǣ������ڲ�������Щ�����Ǳ��������Ѿ�ȷ���Ǳ���������еĵ㣬��maskͬʱҲΪ���  
        //������Ƿָ���ǰ��ͼ��  
        grabCut( *image, mask, rect, bgdModel, fgdModel, 1 );  
    else  
    {  
        if( rectState != SET )  
            return iterCount;  
  
        if( lblsState == SET || prLblsState == SET )  
            grabCut( *image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK );  
        else  
            grabCut( *image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT );  
  
        isInitialized = true;  
    }  
    iterCount++;  
  
    bgdPxls.clear(); fgdPxls.clear();  
    prBgdPxls.clear(); prFgdPxls.clear();  
	
  
    return iterCount;  
}  
  
GCApplication gcapp;  
  
static void on_mouse( int event, int x, int y, int flags, void* param )  
{  
    gcapp.mouseClick( event, x, y, flags, param );  
}  

void CAboutDlg::bblocation()
{
    

}


void CwindowTestDlg::BBlocation()
{
	// TODO: �ڴ���������������
	Mat image;
	if(flage==true){
	image= cv::Mat(SrcImg);
	}
	else{
	string filename = "HSV.jpg";  
    image = imread( filename, 1 );
	}
	cout<<flage;
	
	//HSV();
	//Mat image= cv::Mat(SrcImg);
    if( image.empty() )  
    {  
        cout << "\n Durn, couldn't read image filename " << endl;  

    }  
  
    help();  
  
    const string winName = "image";  
    cvNamedWindow( winName.c_str(), CV_WINDOW_AUTOSIZE );  
    cvSetMouseCallback( winName.c_str(), on_mouse, 0 );  
  
    gcapp.setImageAndWinName( image, winName );  
    gcapp.showImage();
  
    for(;;)  
    {  
        int c = cvWaitKey(0);  
        switch( (char) c )  
        {  
        case '\x1b':  
            cout << "Exiting ..." << endl;  
            goto exit_main;  
        case 'r':  
            cout << endl;  
            gcapp.reset();  
            gcapp.showImage();  
            break;  
        case 'n':  
            int iterCount = gcapp.getIterCount();  
            cout << "<" << iterCount << "... ";  
            int newIterCount = gcapp.nextIter(); 
            if( newIterCount > iterCount )  
            {  
                gcapp.showImage();  
                cout << iterCount << ">" << endl; 
				

            }  
            else  
                cout << "rect must be determined" << endl;  
            break;  
        }  
    }  
	
  
exit_main:  
    cvDestroyWindow( winName.c_str() );  
}


void CwindowTestDlg::Median_filter()
{
	
	Mat image= cv::Mat(SrcImg);
	//imshow("005.jpg",image);
	Mat Salt_Image;  
    image.copyTo(Salt_Image);
	Mat image3, image4;  
	medianBlur(Salt_Image, image4, 3);    
    imshow("��ֵ�˲�", image4);
	imwrite("��ֵ�˲�.jpg", image4);
	SrcImg=&(IplImage) image4;
    waitKey();  

}



Mat img;  
//�Ҷ�ֵ��һ��  
Mat bgr;  
//HSVͼ��  
Mat hsv;  
//ɫ��  
int hmin = 0;  
int hmin_Max = 360;  
int hmax = 78;  
int hmax_Max = 360;  
//���Ͷ�  
int smin = 32;  
int smin_Max = 255;  
int smax = 255;  
int smax_Max = 255;  
//����  
int vmin = 36;  
int vmin_Max = 255;  
int vmax = 255;  
int vmax_Max = 255;  
//��ʾԭͼ�Ĵ���  
string windowName = "src";  
//���ͼ�����ʾ����  
string dstName = "dst";  
//���ͼ��  
Mat dst_HSV;  
//�ص�����  
void callBack()  
{  
    //���ͼ������ڴ�  
    dst_HSV = Mat::zeros(img.size(), CV_32FC3);  
    //����  
    Mat mask;  
    inRange(hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);  
    //ֻ����  
    for (int r = 0; r < bgr.rows; r++)  
    {  
        for (int c = 0; c < bgr.cols; c++)  
        {  
            if (mask.at<uchar>(r, c) == 255)  
            {  
                dst_HSV.at<Vec3f>(r, c) = bgr.at<Vec3f>(r, c);  
            }  
        }  
    }  
    //���ͼ��  
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
    //��ʴ����
	//Mat out;
    //morphologyEx(dst, out, MORPH_OPEN,element);
    imshow(dstName, dst_HSV); 
	//imshow("fushihou", out);
    //����ͼ��  
    dst_HSV.convertTo(dst_HSV, CV_8UC3, 255.0, 0);  
    imwrite("HSV.jpg", dst_HSV);
	//SrcImg=&(IplImage) dst_HSV;
}  

void CwindowTestDlg::hsv_color()
{
	flage=false;
	// TODO: �ڴ��������������� 
	//img = imread("��ֵ�˲�.jpg", IMREAD_COLOR); 
	 img= cv::Mat(SrcImg);
	// img= bsSrc;
    if (!img.data || img.channels() != 3)  
        cout<<"����"<<endl;  
    //imshow(windowName, img);  
    //��ɫͼ��ĻҶ�ֵ��һ��  
    img.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);  
    //��ɫ�ռ�ת��  
    cvtColor(bgr, hsv, COLOR_BGR2HSV);  
    //�������ͼ�����ʾ����  
    //namedWindow(dstName,CV_WINDOW_NORMAL);  
    //����ɫ�� H  
    //createTrackbar("hmin", dstName, &hmin, hmin_Max, callBack);  
    //createTrackbar("hmax", dstName, &hmax, hmax_Max, callBack);  
    ////���ڱ��Ͷ� S  
    //createTrackbar("smin", dstName, &smin, smin_Max, callBack);  
    //createTrackbar("smax", dstName, &smax, smax_Max, callBack);  
    ////�������� V  
    //createTrackbar("vmin", dstName, &vmin, vmin_Max, callBack);  
    //createTrackbar("vmax", dstName, &vmax, vmax_Max, callBack);  
    //callBack(0,0);
	callBack();
    waitKey(0);  
}


void CwindowTestDlg::lunkuo()
{
	Mat src_image = imread("Grabcut.jpg");  
    if(!src_image.data)  
    {  
        cout << "src image load failed!" << endl;  
    }  
    namedWindow("src image", WINDOW_NORMAL);  
    imshow("src image", src_image);  
  
    /*�˴���˹ȥ�������ں����ֵ�������Ч��*/  
    Mat blur_image;  
    GaussianBlur(src_image, blur_image, Size(15, 15), 0, 0);  
    imshow("GaussianBlur", blur_image); 
	imwrite("��˹ȥ��.jpg",blur_image);
  
    /*�Ҷȱ任���ֵ��*/  
    Mat gray_image, binary_image;  
    cvtColor(blur_image, gray_image, COLOR_BGR2GRAY);  
    threshold(gray_image, binary_image, 0, 255, THRESH_BINARY);  
    imshow("binary", binary_image);
	imwrite("��ֵ�任.jpg",binary_image);
  
    /*��̬ѧ�ղ���*/  
    Mat morph_image;  
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));  
    morphologyEx(binary_image, morph_image, MORPH_CLOSE, kernel, Point(-1, -1), 2);  
    imshow("morphology", morph_image); 
	imwrite("��̬ѧ�ղ���.jpg",morph_image);
  
    /*����������*/  
    vector< vector<Point> > contours;  
    vector<Vec4i> hireachy;  
    findContours(morph_image, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());  
    Mat result_image = Mat::zeros(src_image.size(), CV_8UC3);  
    for(size_t t = 0; t < contours.size(); t++)  
    {  
        /*���˵�С�ĸ�������*/  
        Rect rect = boundingRect(contours[t]);  
        if(rect.width < src_image.cols/2)  
            continue;  
        if(rect.width > (src_image.cols - 20))  
            continue;  
  
        /*����������ܳ�*/  
        double area = contourArea(contours[t]);  
        double len = arcLength(contours[t], true);  
  
        drawContours(result_image, contours, static_cast<int>(t), Scalar(0, 0, 255), 1, 8, hireachy);  
        cout << "area of start cloud: " << area << endl;  
        cout << "len of start cloud: " << len << endl;  
    }  
  
    imshow("result image", result_image); 
	imwrite("����.jpg",result_image);
}
