This projects implements a double word aligned heap, with a 4KB size limitation to show understanding on how to avoid false and explicit fragmentation.
Allocated blocks have headers which contain a size and a status
Free blocks have an additional footer that only contains its size to showcase that it's free

Illustration: 


![image](https://user-images.githubusercontent.com/109580376/219672982-08ae328b-bf0d-4d3c-a82d-a3e5ee7616f8.png)
