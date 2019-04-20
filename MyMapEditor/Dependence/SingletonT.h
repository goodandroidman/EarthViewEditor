#ifndef  SINGLETONT_H
#define SINGLETONT_H
/**
*   单例类模板
*   继承之后需要在实现单例类内调用 SINGLETON_DEFINE(class) 宏
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

// 单例类友元类宏定义
#define SINGLETON_DEFINE(classname) friend class SingletonT<classname>;

#endif //SINGLETONT_H