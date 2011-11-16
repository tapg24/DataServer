#ifndef SINGLETON_H
#define SINGLETON_H

template< class T >
class Singleton
{
private:
	Singleton();
	Singleton( const Singleton& );
	Singleton& operator = ( const Singleton& );
	~Singleton() {};

public:

	static T& getInstance()
	{
		static T tmp;
		return tmp;
	}

	struct ForceInit
	{ 
		ForceInit()
		{
			getInstance();
		} 
	};
}; // class Singleton

#endif // SINGLETON_H
