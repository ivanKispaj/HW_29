//
// main.cpp
// 2023-09-02
// Ivan Konishchev
//

#include <iostream>
#include "FineGrainedQueue.h"
#include <thread>

int main(int argc, const char *argv[])
{
	FineGrainedQueue queue;
	int num{1};
	int num1{0};
	for (int i = 0; i < 8; i++)
	{
		std::thread th([&]()
					   {
			queue.inserAt(i, num);
			num++; });
		if (i % 2)
		{
			std::thread th1([&]()
							{ 
				queue.remove(num1);
				num1 += 2; });
			th1.join();
		}

		th.join();
	}
    std::cout << queue;
	queue.inserAt(0, num);
	num++;
	queue.inserAt(1, num);
	num++;
	queue.inserAt(2, num);
	num++;
	queue.inserAt(3, num);
	num++;
	queue.inserAt(10, num);
    std::cout << queue;
	queue.remove(30);
	num -= 2;
	queue.remove(num - 2);
    std::cout << queue;
	return 0;
}
