# assignment 1-1

![image-20250401153618643](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401153618643.png)

![image-20250401153626994](C:\Users\31631\AppData\Roaming\Typora\typora-user-images\image-20250401153626994.png)

虽然单精度只需要4字节空间，但是在使用printf的时候，会自动转换为 double，导致只读取到没有定义的0，需要转换成double进行输出。

无法使用直接压栈的方法来进行操作，于是借助FPU来存储浮点数，预留好空间之后在调用printf

借此虽然double类型可以分两次通过高四位和低四位直接压入，但不符合规范，于是也采用FPU操作。

longdouble为了契合printf的格式与float同理