#ifndef  SINGLETONT_H
#define SINGLETONT_H
/**
*   ������ģ��
*   �̳�֮����Ҫ��ʵ�ֵ������ڵ��� SINGLETON_DEFINE(class) ��
*/
template<typename T>
class SingletonT
{
public:
	virtual ~SingletonT() { 
	}

	static T* getSingletonPtr()
	{
		static T instance;
		return &instance;
	}

	static T& getSingleton() { return *getSingletonPtr(); }

protected:
	SingletonT(){}
	SingletonT(const SingletonT&);
	SingletonT& operator=(const SingletonT&);
};

// ��������Ԫ��궨��
#define SINGLETON_DEFINE(classname) friend class SingletonT<classname>;

#endif //SINGLETONT_H