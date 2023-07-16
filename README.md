# Hisilicon
## 作品简介
海洋馆是一个充满生机和活力的地方，物种多样性十分丰富。目前大部分海洋馆采用标识牌简要介绍海洋生物，但海洋生物种类繁多且复杂，游客很难对某一种感兴趣的海洋生物进行深入了解，这将极大影响游客在海洋馆的全面体验。

为进一步提高海洋馆的科技感和互动性，让游客更加深入地了解海洋生物，我们设计了一款基于AI视觉的海洋馆导游机器人。通过Taurus采集图像数据识别海洋生物，并进行语音播报；通过Pegasus控制机器人运动，在OLED屏幕展示二维码等功能，提供“看得见、认得出、会判断、能讲解”的智能化服务。
## 代码介绍
Pegasus/robot_car/目录下面是控制小车运动的代码，小车有循迹和停止两种工作模式，通过按键进行切换。

Pegasus/interconnection_oled/目录下面是Pegasus进行串口通信的代码：Taurus通过串口将识别结果反馈给Pegasus端，Pegasus接收到Taurus传过来的数据，在OLED屏幕展示相应生物的二维码。

Taurus/cnn_trash_classify/目录下面是海洋生物分类的关键代码。

Taurus/aac_file/目录下面是语音播报的aac文件。

Taurus/interconnection_server/目录下面是Taurus端串口通信的关键代码。

Taurus/fishdata_inst.wk是训练好的wk模型。
