#include "cloud_backup.hpp"
#include <thread>

void thr_hotfile()
{
  CloudBackup::FileManageModule fm;
  fm.start();
}
void thr_httpserv()
{
  CloudBackup::HttpServerModule srv;
  srv.start();
}

int main(int argc, char *argv[])
{
  std::thread hotfile(thr_hotfile);	// C++11中的线程-启动非热点文件压缩模块
  std::thread httpserv(thr_httpserv);	// 网络通信服务端模块启动
  hotfile.join();	// 等待线程退出
  httpserv.join();

  return 0;
}
