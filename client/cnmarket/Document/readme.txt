
    ���ð�����: http://www.zlinker.com/src

    ��ϵ��ʽ: support@zlinker.com

�������⣺

1��Դ���뾭��̺������Ŀ���ļ�ִ�к���ʾΪ���ð棬�����Ƿ�����������л���רҵ�棿

    ��ʾΪ�������������ģ�ע����л���רҵ�档
    Tools���̰���ע�����Tools��ִ�е�ʱ����ҪStkLib.dll,StkNet.dll,GuiLib.dll,
XPFace.dll�ĸ���̬�⡣
    ������ڱ����ֱ��Ϊרҵ�棬���޸�һ��StkNet\Include\Secret.inl �ĺ���
CSView::ValidProf(...),�ں�����ͷֱ�Ӽ���һ�� return TRUE;
    ����뽫�汾����Ϊ��ר�Ұ桱����쫷�桱�ȣ����޸�StkNet/Src/AfxCore.cpp�ļ��е�
CString AfxGetVersionString( )������

2����δ���װ���裿

    ���ִ�л�����Ҫ�ĵ�����Install\SetupĿ¼�¡�ִ�л�����Ҫ����һЩ��Ʊ��ʼ����
�Լ������StkUI\release\�µ������ļ���
    StkLib.dll  StkNet.dll GuiLib.dll ProfUIS.dll XPFace.dll StockAna.exe
    �Լ�Unzip.dll����dllΪ��zip���ã�Ŀǰ��Դ���룬ȱ�����dllҲû�й�ϵ����Ӱ����
����ִ�к�ʹ�ã�ֻ�ǲ����Զ���zip���ˣ���
    ���⣬��Ҫmfc42.dll��msvcrt.dll����MFC�⡣

3���������ĳ������̱���������Ҫ�����²��֣����������Ϊ�����������ͻ��

	0. �ظģ�Src\StkLib\Include\StkLib.h �е�һЩOEM�泣������
	1. �ظģ���������ļ���׺����

	2. �޸ģ�������ƣ���������̣�www.zlinker.net����Ѷ��վ
	3. ȥ�����޸ģ��˵��е��Զ�����ȥ������ι���ȥ������Ʒ��Ϣ��վȥ��
	4. ȥ�����޸ģ�������ι���ť���������Ϲ���ť
	5. �޸İ�����������ƣ���������̣�ȥ������ҳ��ȥ������ͼ��ȥ��ʮ����ɫ��ȥ
��ÿҳ���������
	6. �޸�����̱꣬���ͼ
	7. �޸�Readme.txt, License.txt

4������޸����ȱʡ������

    �޸��ļ�Src\StkLib\Src\Profile.cpp�еĹ��췽��
	CColorClassContainer::CColorClassContainer( )
    �е�m_nSelectedColorClass	=	1һ�У����Խ�m_nSelectedColorClass��ֵΪ0��
1��2��3���ֱ����ͬ��ȱʡ��ɫ���

    ����ÿ��ȱʡ������ɫ���ã������޸����·�������Ӧ��ֵ�������޸�
	void CColorClass::SetColorDefault( )
	void CColorClass::SetColorStandard( )
	void CColorClass::SetColorByzantinesque( )
	void CColorClass::SetColorGothic( )
	void CColorClass::SetColorBaroque( )

    �޸��ļ�Src\StkUI\StkUI.cpp����CStkUIApp::InitInstance()���´��룺
	AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
    ���ɸı����������ҳ

5��ִ��ʵʱ�������ʱ���֡��޷����ؽ���ϵͳ�����Ƿ�ȱ��Stock.dll��ʵʱ������ճ���ͨ��
���⣬��������Щ����Դ��

    ʵʱ�������ʹ��ͨ�ӿ��ӿں��廪���¿��ӿڡ�Ŀǰͨ�ӽӿڲ���Ϊ��׼��
    Stock.dll��ͨ�ӽӿڵĿ⣬��Ҫ��װͨ�ӿ����߼���ͨ�ӵ���������Ż��С�

    ͨ�ӿ��ǽ������ߵ�������㲥�Ĺ�Ʊ���ݵ�Ӳ�����տ���Stock.dll��������������������
�������ǽ�Internet����������ת����ͨ�ӽӿ����ݡ�
   
    Ӳ�����տ���ȱ���Ǳ������գ�ʹ���������ʱ�������Ҫ��ʷ���ݣ���Ҫ�ֹ����ء�

    ������Ͻӿڲ�����������Ҫ����������Ҫ������Ʊʵʱ�����������

6����Ȩ�ļ���ʽ����θ��£�

    �����Ȩ��Ϣ������ļ� data\CHNA.pwr ��CHNA.pwr ������ң�http://www.fxj.com.cn��
    ��Ȩ�ļ���ʽ��ͬ��

    ���⣬�û����С����ݹ����޸ĺ�ĳ�Ȩ��Ϣ������data\shase\xdrdata\ �� 
    data\sznse\xdrdata�£�ÿ����Ʊһ���ļ�����ʽΪ struct drdata_t �����顣

    ��Ȩ���ϸ��¿������������ʷ���ء����Զ����ظ��£�Ҳ���Ե�������ҳ�Ȩ�ļ���

7�����������ļ�����θ��£�

    ���������Ϣ������ļ� data\CHNA.bst �ļ��� CHNA.bst �ļ�Ϊ struct basedata_t ��
    ���顣

    �������ϸ��¿������������ʷ���ء����Զ����ظ��£�Ҳ���Ե���ͨ�������Ŀ¼�µ�
    base.dbf �ļ����и��¡�

    ͨ����������أ�http://www.sostock.com.cn/hq/ ���س�ǿ���������

8��ע������ʹע��������ǰ�������ͬ��

    �޸�StkLib\Include\StkLib.h�У������ĸ������Ķ��岢���±����ע���뼴�����ȫ��ͬ��
    ע�ⳤ�Ȳ�Ҫ����5

      #define OEM_SEED_STD1    "dc./x"             // ��׼��ע�������1
      #define OEM_SEED_STD2    "sfk%c"             // ��׼��ע�������2
      #define OEM_SEED_PROF1   "78cd2"             // רҵ��ע�������1
      #define OEM_SEED_PROF2   "d6$3#"             // רҵ��ע�������2

    ͬʱҪȥ�����¶����ע��
      #define	CLKVER_OEM		// OEM��ȥ������ע��
