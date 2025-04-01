# assignment 1-2

在对未初始化的数据进行输入的时候，并不需要再为他分配空间，因为在定义未初始化数据的时候，已经给每个数据分配好内存了。

![image-20250401183146806](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401183146806.png)

![image-20250401183045220](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401183045220.png)

在调用scanf的时候，会自动把输入的数据存到对应的内存中，打印的时候也可以通过data找到对应地址中存的数据。

![image-20250401202757393](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401202757393.png)

存入的是地址，所以只需要4位，但打印的时候需要补全，14位的longdouble需要补到16位。

![image-20250401202904645](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401202904645.png)