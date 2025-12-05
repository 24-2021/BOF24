## 参考项目

原项目：https://github.com/trustedsec/CS-Situational-Awareness-BOF

参考以上项目增加了一些api，写了一些自己习惯用的一些信息收集命令的BOF



## BOF 功能列表

| 模块 | 功能 | 入口 | 参数 |
|---|---|---|---|
| `chdir` | 修改当前工作目录并输出当前目录 | `bof24/chdir/entry.c:112` | `[path]` 可选 |
| `dir` | 列出当前目录的文件与目录，含时间和大小统计；支持 UNC 尾部补全 | `bof24/dir/entry.c:6`, `bof24/dir/entry.c:110` | 无 |
| `netstat` | 列出 TCP/UDP IPv4 端点，显示状态、进程名与 PID | `bof24/netstat/entry.c:132`, `bof24/netstat/entry.c:159` | 无 |
| `tasklist` | 列出进程，显示图像名、PID、PPID | `bof24/tasklist/entry.c:5`, `bof24/tasklist/entry.c:18` | 无 |
| `netlocalgroup_admins` | 列出本机 Administrators 本地组成员（域\用户名） | `bof24/netlocalgroup_admins/entry.c:6`, `bof24/netlocalgroup_admins/entry.c:27-31` | 无 |
| `netshare` | 列出本机共享（名称、备注） | `bof24/netshare/entry.c:6`, `bof24/netshare/entry.c:27-31` | 无 |
| `netstart` | 列出已启动的 Win32 服务（显示名或服务名） | `bof24/netstart/entry.c:8`, `bof24/netstart/entry.c:57-61` | 无 |
| `netuser` | 列出本机普通用户账户名 | `bof24/netuser/entry.c:6`, `bof24/netuser/entry.c:27-31` | 无 |
| `rdp_port` | 显示 RDP 端口号（注册表 HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp\PortNumber） | `bof24/rdp_port/entry.c:7-25` | 无 |
| `wd_add_exclusion` | 向 Windows Defender 添加路径排除 C:\\users\\public（优先常规键，失败则写入策略键） | `bof24/wd_add_exclusion/entry.c:5-68` | 无 |
| `wmic_product` | 使用 WMI 枚举安装的产品名称与版本（Win32_Product） | `bof24/wmic_product/entry.c:6-31` | 无 |
| `fsinfo_drives` | 列出本机逻辑盘符（类似 fsutil fsinfo drives） | `bof24/fsinfo_drives/entry.c:1-18` | 无 |
| `wmic_memorychip` | 列出物理内存条容量与速度（Win32_PhysicalMemory） | `bof24/wmic_memorychip/entry.c:6-31` | 无 |
| `wmic_logicaldisk` | 列出逻辑磁盘的总容量与可用空间（Win32_LogicalDisk） | `bof24/wmic_logicaldisk/entry.c:6-31` | 无 |
| `wmic_videocontroller` | 列出视频控制器名称（Win32_VideoController） | `bof24/wmic_videocontroller/entry.c:6-31` | 无 |
| `wmic_qfe` | 列出已安装补丁 HotFixID 与时间（Win32_QuickFixEngineering） | `bof24/wmic_qfe/entry.c:6-31` | 无 |
| `wlan_profiles` | 列出无线配置文件（类似 netsh wlan show profiles） | `bof24/wlan_profiles/entry.c:1-28` | 无 |
| `recent_files` | 列出 `%APPDATA%\Microsoft\Windows\Recent` 下的最近项 | `bof24/recent_files/entry.c:1-17` | 无 |
| `net_use` | 列出已连接/记忆的网络资源（类似 net use） | `bof24/net_use/entry.c:1-22` | 无 |
| `domain_trusts` | 列出域信任信息（支持筛选：PRIMARY/FOREST/DIRECT_OUT/ALL_TRUSTS） | `bof24/domain_trusts/entry.c:18-34,41-49` | `[PRIMARY|FOREST|DIRECT_OUT|ALL_TRUSTS]` 可选 |
| `net_config_workstation` | 显示工作站配置信息（类似 net config workstation） | `bof24/net_config_workstation/entry.c:1-16` | 无 |
| `net_user_domain` | 列出域用户（类似 net user /domain） | `bof24/net_user_domain/entry.c:1-38` | 无 |
| `net_group_domain` | 列出域全局组（类似 net group /domain） | `bof24/net_group_domain/entry.c:1-37` | 无 |
| `net_group_members` | 列出指定域组成员（类似 net group "<组名>" /domain） | `bof24/net_group_members/entry.c:1-45` | `[group]` 必选 |
| `net_time_domain` | 显示域控时间（类似 net time /domain） | `bof24/net_time_domain/entry.c:1-21` | 无 |
| `wmic_useraccount` | 使用 WMI 枚举用户账号信息（Win32_UserAccount） | `bof24/wmic_useraccount/entry.c:1-20` | 无 |

## 声明

该项目仅用于安全学习与交流，请勿用于违法及其他不合规活动
