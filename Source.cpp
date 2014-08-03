#include<iostream>
#include<string>
using namespace std;
template<typename T>
class object_pool
{
public:
	static void* operator new(size_t size);
	static void operator delete(void* p, size_t size);
	virtual ~object_pool(){};		//��ס��������Ϊ�������������һ��Ҫ���麯����������_nextָ��ʲô����Ҫ�����������ӣ�
private:
	static void add_to_free_list(T *p);
	static void free_from_pool(T *p);
protected:
	T *_next;		//����Mixin��ģʽ��Ҫ�����pool����Ҫ�̳�����������Ҳ������nextָ�룻��Ϊ�Ǽ̳еģ�������protected��
private:
	static T *_next_free;
	static size_t _size;
};
template<typename T>
T* object_pool<T>::_next_free = nullptr;

template<typename T>
size_t object_pool<T>::_size = 64;

template<typename T>
void object_pool<T>::add_to_free_list(T* p)
{
	p->_next = _next_free;
	_next_free = p;
}

template<typename T>
void object_pool<T>::free_from_pool(T *p)
{
	
}

template<typename T>
void object_pool<T>::operator delete(void *p, size_t size)
{
	if(p == nullptr) 
	{
		return ;
	}
	if(sizeof(T) != size)
	{
		::operator delete(p);
		return ;
	}
	free_from_pool(static_cast<T*>(p));
	add_to_free_list(static_cast<T*>(p));
}

template<typename T>
void* object_pool<T>::operator new(size_t size)
{
	if(sizeof(T) != size)
	{
		return ::operator new(size);
	}
	if(_next_free == nullptr)
	{
		T *new_block = static_cast<T*>(::operator new(sizeof(T)*_size));
		for(size_t i = 0; i < _size; ++i)
		{
			add_to_free_list(&new_block[i]);
		}
		new_block[_size - 1]._next = nullptr;
	}
	T *res = _next_free;
	_next_free = res->_next;
	return res;
}

class Person:public object_pool<Person>
{
public:
	Person(string _name = "nobody"):name(_name){}
	string get_name() const
	{
		return name;
	}
private:
	string name;
};

int main()
{
	Person *xyb = new Person("xyb");
	Person *zpl = new Person("zpl");
	cout<<xyb->get_name()<<endl;
	cout<<zpl->get_name()<<endl;
	return 0;
}