Create a 64 MB disk image file full of zeros
```
dd if=/dev/zero of=mydisk.img bs=1M count=64
```

or if that doesnt work use this to run with bash explicity, not sure why ive had problems with above
```
bash -c "dd if=/dev/zero of=mydisk.img bs=1M count=64"
```

Format it as FAT32
```
mkfs.fat -F 32 mydisk.img
```

Mount it so you can put files on it
```
mkdir /tmp/mnt
sudo mount -o loop mydisk.img /tmp/mnt
```

Copy some files in
```
sudo cp ~/somefile.txt /tmp/mnt/
sudo umount /tmp/mnt
```
