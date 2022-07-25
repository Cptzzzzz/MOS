## OS第三次理论作业

#### T1

代码如下

```python
import matplotlib.pyplot as plt

pages=[0, 9, 8, 4, 4, 3, 6, 5, 1, 5, 0, 2, 1, 1, 1, 1, 8, 8, 5, 
3, 9, 8, 9, 9, 6, 1, 8, 4, 6, 4, 3, 7, 1, 3, 2, 9, 8, 6, 2, 9, 2, 7, 2, 7, 8, 4, 2, 3, 0, 1, 9, 4,
7, 1, 5, 9, 1, 7, 3, 4, 3, 7, 1, 0, 3, 5, 9, 9, 4, 9, 6, 1, 7, 5, 9, 4, 9, 7, 3, 6, 7, 7, 4, 5, 3, 5, 3, 1, 5, 6, 1, 
1, 9, 6, 6, 4, 0, 9, 4, 3]
def opt():
    res=[]
    for n in range(1,11):
        frame=[-1 for i in range(n)]
        cnt=0
        for i in range(len(pages)):
            if pages[i] in frame:
                pass
            elif -1 in frame:
                cnt+=1
                frame[frame.index(-1)]=pages[i]
            else:
                cnt+=1
                ind=[]
                rest=pages[i+1:]
                for val in frame:
                    if val not in rest:
                        ind.append(1000)
                    else:
                        ind.append(rest.index(val))
                vindex=ind.index(max(ind))
                frame[vindex]=pages[i]
        res.append(cnt)
    return res
def lru():
    res=[]
    for n in range(1,11):
        que=[]
        cnt=0
        for i in pages:
            if i in que:
                pass
            else:
                cnt+=1
                if len(que)==n:
                    que.pop(0)
                    que.append(i)
                else:
                    que.append(i)
        res.append(cnt)
    return res
def fifo():
    res=[]
    for n in range(1,11):
        time=[0 for i in range(10)]
        frame=[-1 for j in range(n)]
        cnt=0
        for i in pages:
            for j in range(10):
                time[j]+=1
            time[i]=0
            if i not in frame:
                cnt+=1
                if -1 in frame:
                    frame[frame.index(-1)]=i
                else:
                    indd=0
                    for j in range(n):
                        if time[frame[j]]>time[frame[indd]]:
                            indd=j
                    frame[indd]=i
        res.append(cnt)
    return res
x=[i for i in range(1,11)]
OPT=opt()
LRU=lru()
FIFO=fifo()
plt.plot(x,OPT,label="OPT")
plt.plot(x,LRU,label="LRU")
plt.plot(x,FIFO,label="FIFO")
plt.xlim(0,11)
plt.xticks(x)
plt.legend()
for i in range(1,11):
    plt.text(i,OPT[i-1],str(OPT[i-1]))

    plt.text(i,LRU[i-1],str(LRU[i-1]))
    plt.text(i,FIFO[i-1],str(FIFO[i-1]))
plt.show()

```

三个置换算法的结果分别为（n从1-10）

OPT：[90, 64, 48, 37, 29, 22, 16, 12, 11, 10]
LRU：[90, 80, 67, 59, 47, 39, 30, 20, 12, 10]
FIFO：[90, 79, 71, 58, 52, 42, 28, 17, 13, 10]

![fig1](https://raw.githubusercontent.com/Cptzzzzz/image/image/fig1.png)

#### T2

在32位4GB地址空间中采用二级页表机制，页内偏移占12位，代表着每一页大小为4KB，因此4GB地址空间一共被分为2^20个页，每个页需要一个页表项，而页表空间占用了4MB，因此每个页表项占用了4B。采用二级页表机制，页目录（第一级页表）占用10位地址，可以检索到1024个页表，每个页表检索1024个页面，因而第二级页表占用10位地址。页目录本身占用4KB空间，说明每个页目录项与每个页表项一样，占用4B内容，而页目录本身恰好是一个页面的大小，因此可以使用页目录自映射机制，将页目录恰好安置在一个页面上，该页称作页目录页；每个页表也恰好安置在一个页面上，该页称作页表页。

将4MB的第二级页表看作对整个4GB内存空间的压缩映射，其起始地址是0x80000000，那么所有1024个第二级级页表页中，必然有一个页表页映射的是4MB第二级页表空间，这个页表页和页目录的功能和内容完全重合，将其作为页目录即可节省4KB一个页面的页目录空间，这就是页目录自映射机制。

4MB第二级页表空间对于整个4GB内存空间是线性映射的，其起始地址是0x80000000，那么要寻找页目录起始地址，只需要知道页目录所映射的内存相对于整个内存布局在什么位置，再加上第二季页表起始地址即可。页目录所映射的就是第二级页表空间，其起始地址是0x8000_0000，其之前有0x80000000 >> 12 = 0x80000个页，每个页在页表中占4B的页表项空间，所以页目录所映射的内存相对于整个内存在第二级页表中的偏移量为 0x80000 * 4 = 0x200000。页目录起始地址即为 0x80000000 + 0x200000 = 0x80200000。

综上总结，在32位4GB地址空间，4KB页面大小，采用二级页表机制，其页表起始地址和页目录起始地址的关系为：$PD_{base} = PT_{base}+ PT_{base}> >10 $。在0x80000000处映射页表，那么页目录起始地址为0x80200000。

#### T3

1.进程整个空间地址共4GB，页内偏移12位，每一页4KB

2.根据上图的页目录和页表内容，访问0x0时先查页目录项0000：有效标志为0，页面尚未装入，引发缺页中断。

根据上图的页目录和页表内容，访问0x00803004 = 0b00000000100000000011000000000100，页目录位0b0000000010，查页目录项0002：有效标志为1，页表物理地址0x5000。原地址页表位0b0000000011，查0x5000处的页表项0003：0x20001，有效标志为1，页面物理地址为0x20000。原地址页内偏移位0b00000000_0100，系统为大端和小端访问到的数据分别为0b00000000和0b00000001。

根据上图的页目录和页表内容，访问0x00402001 = 0b00000000010000000010000000000001，页目录位0b0000000001，查页目录项0001：有效标志为1，页表物理地址0x1000。原地址页表位0b0000000010，查0x1000处，即页目录本身的页表项（页目录项）0002：0x5001，有效标志为1，页面物理地址为0x5000。原地址页内偏移位0b000000000001，系统为大端和小端访问到的数据都会是0b00000000。

3.若要访问物理地址0x326028，该物理地址的低12位0x028是页内偏移，物理页框号0x326000，可知虚拟地址页内偏移位为0b000000101000。在上图所示的页表内容中查到0x20000处页表的0001偏移处存有该物理页框号且有效位为1，可知虚拟地址的页表偏移位为0b0000000001。从页目录中查到页目录项0003中页表物理地址为0x20000且标志位为1，可知虚拟地址页目录位为0b000000_0011。综上，虚拟地址为0b00000000110000000001000000101000，即0x00c01028.
