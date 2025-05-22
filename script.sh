export DEBIAN_FRONTEND=noninteractive
apt-get update
apt install -y curl vim git build-essential

cat /home/vagrant/.ssh/me.pub >>/home/vagrant/.ssh/authorized_keys
mkdir -p /root/.ssh

cat /home/vagrant/.ssh/me.pub >>/root/.ssh/authorized_keys

