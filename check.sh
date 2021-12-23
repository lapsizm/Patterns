sudo rm -rf cmake-build-*
git submodule update --init
sudo docker run -v `pwd`:`pwd` -w `pwd` --cap-add=SYS_PTRACE -t rusdevops/bootstrap-cpp scripts/checks.sh && sudo docker run -v `pwd`:`pwd` -w `pwd` -t rusdevops/bootstrap-cpp scripts/tests.sh
sudo rm -rf _builds/
