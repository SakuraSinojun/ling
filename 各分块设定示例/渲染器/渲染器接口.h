

/*******************************************************************************

	ͼ�����棨��Ⱦ�����ӿڣ�

*******************************************************************************/



// ��Ҫ��Ⱦ���̣�����������ܻᱻOnPaint���ã����Ե���һ�¡�
void RenderScene(void);


// �󶨵�һ�����ڣ���������Ⱦ�̣߳��߳��ѿ�ʼ������render_start�ı��ʼ״̬��
// ����̲߳�����SUSPENDED�ģ���Ӧ���Ǹ���״̬�������ı�����Ⱦ���ǵȴ���
BOOL render_attach(HWND);

// ��ʼ��Ⱦ�̡߳�
BOOL render_start();

// ֹͣ��Ⱦ�̣߳��߳���ʽ������
// ִ������������󣬿�ʼ�������߳�Ӧ�ð�ȫ������
BOOL render_stop();

// ��ͣ��Ⱦ�̣߳�����CPUʱ��Ƭ��
// ���ڴ�����С����ʧȥ���㡢�л�GDI�˵��������
BOOL render_pause();

// ֹͣ�̺߳����������Ĵ���
BOOL render_detach();


// ����һ�����壬����ֵ����������������
// �������δ�����ö���һ����˵ֻ����һ�������������������塣
// bufferָ���Ӧ����һ�������Ļ��壬len��ʾ���������ȡ�
// typeָ����λ�����������͡�
// ģ���е���ƷӦ���ж��Ƿ���������Ұ���Ծ����Ƿ���ơ�
int render_create_object(void* buffer, int len, int type);


// ����һ����������۲��ߵ�ƽ�档���ƽ���������ģ���ı������ڡ��˵��ȡ�
int render_create_panel(void * buffer, int len, int type);


// ���Ƶ��档ѡ����ʵĵ��淽���С���Լ��ټ������λ�õĹ�������
// ���ǣ����淽��������󣬻���ֵ���ʼ�ջ��Ƶ������
int render_create_map(void * buffer, int len, int type);


// ����������������ݡ�
// ����type�����Ϳ��Խ����������Ϊ����Ĳ��䡢����֡������ͼ���ʵȵȡ�
// ����ֵ�������������������������е�������
int render_add_data(int object, void * buffer, int len, int type);

// �������˶���֡��������������л�����Ӧ��֡��ģ�⶯��Ч����
// ����indexΪrender_add_data()�����ķ���ֵ�������ݶε�������
// ����ִ�гɹ���������һ֡������ֵ������Ѿ������һ֡������0��
// ִ��ʧ�ܣ�����-1
int render_switch_data(int index);


// ��һ������ֱ���ƶ���ĳ��λ�á�
int render_move_2i(int object, int x, int z);
int render_move_2f(int object, float x, float z);
int render_move_3i(int object, int x, int y, int z);
int render_move_3f(int object, float x, float y, float z);

// ��ת���塣��������ת��
// ������������ۼƵ�ǰ��תֵ��ÿ�ε���ʱ��������������ת�Ƕȡ�
// ���ԣ�Ӧ�þ�����3Dģ���ļ��е����尴��׼����ϵ�ڷš�
// ����ָ����ת�ᣬ�������(0, 1, 0)Ϊ��(��y������)��ת��
int render_rotate_1d(int object, double angle);
int render_rotate_3d(int object, double angle, double x, double y, double z);

// �������塣
int render_scale(int object, double scale);

// ɾ�����塣����ֵ��ʣ������������
int render_delete_object(int object);



// �ƶ��ӽǡ�
// ע��������겢���ǽ�ɫ���ꡣ
int render_move_camera_2d(double x, double z);
int render_move_camera_3d(double x, double y, double z);






























