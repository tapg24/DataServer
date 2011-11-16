#ifndef locale_h__
#define locale_h__

struct LocaleSaveRestore_C
{
	LocaleSaveRestore_C ()
	{
		setlocale(LC_ALL, "C");
	}
	~LocaleSaveRestore_C()
	{
		setlocale(LC_ALL, "");
	}
};

#endif // locale_h__
