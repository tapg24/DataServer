#ifndef frl_opc_impl_access_control_h_
#define frl_opc_impl_access_control_h_
#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "../../opc/opc_foundation/opcda.h"
#include "os/win32/com/frl_os_win32_com_allocator.h"
#include <IAccess.h>

namespace frl{ namespace opc{ namespace impl{

	class AccessControl	: public IAccessControl, public os::win32::com::Allocator
	{
	private:
		// reference counter
#if( FRL_COMPILER == FRL_COMPILER_MSVC )
		volatile LONG refCount;
#else
		LONG refCount;
#endif

	public:

		virtual HRESULT STDMETHODCALLTYPE GrantAccessRights( 
			/* [in] */ __RPC__in PACTRL_ACCESSW pAccessList);

		virtual HRESULT STDMETHODCALLTYPE SetAccessRights( 
			/* [in] */ __RPC__in PACTRL_ACCESSW pAccessList);

		virtual HRESULT STDMETHODCALLTYPE SetOwner( 
			/* [in] */ __RPC__in PTRUSTEEW pOwner,
			/* [in] */ __RPC__in PTRUSTEEW pGroup);

		virtual HRESULT STDMETHODCALLTYPE RevokeAccessRights( 
			/* [in] */ __RPC__in LPWSTR lpProperty,
			/* [in] */ ULONG cTrustees,
			/* [size_is][in] */ __RPC__in_ecount_full(cTrustees) TRUSTEEW prgTrustees[  ]);

		virtual HRESULT STDMETHODCALLTYPE GetAllAccessRights( 
			/* [in] */ __RPC__in LPWSTR lpProperty,
			/* [out] */ __RPC__deref_out_opt PACTRL_ACCESSW_ALLOCATE_ALL_NODES *ppAccessList,
			/* [out] */ __RPC__deref_out_opt PTRUSTEEW *ppOwner,
			/* [out] */ __RPC__deref_out_opt PTRUSTEEW *ppGroup);

		virtual HRESULT STDMETHODCALLTYPE IsAccessAllowed( 
			/* [in] */ __RPC__in PTRUSTEEW pTrustee,
			/* [in] */ __RPC__in LPWSTR lpProperty,
			/* [in] */ ACCESS_RIGHTS AccessRights,
			/* [out] */ __RPC__out BOOL *pfAccessAllowed);

	}; // class EnumOPCItemAttributes

} // namespace impl
} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
#endif // frl_opc_impl_access_control_h_
