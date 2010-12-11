

/*******************************************************************************

	图形引擎（渲染器）接口：

*******************************************************************************/



// 主要渲染过程，这个函数可能会被OnPaint调用，所以导出一下。
void RenderScene(void);


// 绑定到一个窗口，并创建渲染线程（线程已开始，但需render_start改变初始状态）
// 这个线程并不是SUSPENDED的，而应该是根据状态变量来改变是渲染还是等待。
BOOL render_attach(HWND);

// 开始渲染线程。
BOOL render_start();

// 停止渲染线程（线程正式结束）
// 执行完这个函数后，开始建立的线程应该安全结束。
BOOL render_stop();

// 暂停渲染线程，交出CPU时间片。
// 用于窗口最小化、失去焦点、切换GDI菜单等情况。
BOOL render_pause();

// 停止线程后，脱离依附的窗口
BOOL render_detach();


// 创建一个物体，返回值是这个物体的索引。
// 这个函数未最终敲定，一般来说只用这一个函数来建立各种物体。
// buffer指向的应该是一段连续的缓冲，len表示缓冲区长度。
// type指定这段缓冲的数据类型。
// 模型中的物品应该判断是否会出现在视野内以决定是否绘制。
int render_create_object(void* buffer, int len, int type);


// 创建一个总是面向观察者的平面。这个平面可以用来模拟文本、窗口、菜单等。
int render_create_panel(void * buffer, int len, int type);


// 绘制地面。选择合适的地面方格大小，以减少计算地面位置的工作量。
// 但是，地面方格如果过大，会出现地面始终绘制的情况。
int render_create_map(void * buffer, int len, int type);


// 向已有物体添加数据。
// 根据type的类型可以将这段数据视为顶点的补充、动画帧或是贴图材质等等。
// 返回值是新增的数据在物体数据链中的索引。
int render_add_data(int object, void * buffer, int len, int type);

// 如果添加了动画帧，这个函数可以切换到相应的帧以模拟动画效果。
// 参数index为render_add_data()函数的返回值，即数据段的索引。
// 数果执行成功，返回下一帧的索引值。如果已经是最后一帧，返回0。
// 执行失败，返回-1
int render_switch_data(int index);


// 将一个物体直接移动到某个位置。
int render_move_2i(int object, int x, int z);
int render_move_2f(int object, float x, float z);
int render_move_3i(int object, int x, int y, int z);
int render_move_3f(int object, float x, float y, float z);

// 旋转物体。按弧度旋转。
// 这个函数并不累计当前旋转值，每次调用时都会重新设置旋转角度。
// 所以，应该尽量把3D模型文件中的物体按标准坐标系摆放。
// 若不指定旋转轴，物体会以(0, 1, 0)为轴(即y坐标轴)旋转。
int render_rotate_1d(int object, double angle);
int render_rotate_3d(int object, double angle, double x, double y, double z);

// 缩放物体。
int render_scale(int object, double scale);

// 删除物体。返回值是剩余物体总数。
int render_delete_object(int object);



// 移动视角。
// 注意这个坐标并不是角色坐标。
int render_move_camera_2d(double x, double z);
int render_move_camera_3d(double x, double y, double z);






























