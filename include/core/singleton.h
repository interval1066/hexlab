#ifndef SINGLETON_H
#define SINGLETON_H

// Thread UNSAFE singleton
//
namespace core
{
	template<class T>
	class Singleton
	{
		Singleton(const Singleton&);
		Singleton& operator= (const Singleton&);

	public:
		static T& Instance();

	protected:
		Singleton(){}
		virtual ~Singleton(){}	
	};

	template<class T>
	T& Singleton<T>::Instance()
	{
		static T instance;
		return instance;
	}
}

#endif

