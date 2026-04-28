#!/bin/bash

set -e

echo "正在检查安装环境..."

if [ ! -e /dev/uinput ]; then
    echo "错误: /dev/uinput 不存在"
    exit 1
fi

echo "检查 /dev/uinput 写权限..."

if [ ! -w /dev/uinput ]; then
    echo "错误: 当前用户没有 /dev/uinput 的写权限"
    echo ""
    echo "请执行以下步骤："
    echo "  1. 将当前用户添加到 input 组: sudo usermod -aG input \$USER"
    echo "  2. 重新登录系统使权限生效"
    echo "  3. 重新运行此安装脚本"
    exit 1
fi

echo "权限检查通过"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SERVICE_FILE="$PROJECT_DIR/config/numlockd.service"

if [ ! -f "$SERVICE_FILE" ]; then
    echo "错误: 未找到 numlockd.service 文件"
    exit 1
fi

echo "复制 numlockd.service 到 /etc/systemd/system/..."
sudo cp "$SERVICE_FILE" /etc/systemd/system/

echo "重新加载 systemd 配置..."
sudo systemctl daemon-reload

echo "启用并启动 numlockd 服务..."
if sudo systemctl enable --now numlockd.service; then
    echo ""
    echo "==================================="
    echo "安装成功！"
    echo "==================================="
    echo ""
    echo "服务状态:"
    sudo systemctl status numlockd.service --no-pager
else
    echo ""
    echo "==================================="
    echo "安装失败！"
    echo "==================================="
    exit 1
fi
