

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 08:44:07 2038
 */
/* Compiler settings for C:\VisualStudio2017Projects\02-COM\05-Automation\ExeServer\Server\ProxyStub\ProxyStub.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ProxyStubHeader_h__
#define __ProxyStubHeader_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISum_FWD_DEFINED__
#define __ISum_FWD_DEFINED__
typedef interface ISum ISum;

#endif 	/* __ISum_FWD_DEFINED__ */


#ifndef __ISubtract_FWD_DEFINED__
#define __ISubtract_FWD_DEFINED__
typedef interface ISubtract ISubtract;

#endif 	/* __ISubtract_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISum_INTERFACE_DEFINED__
#define __ISum_INTERFACE_DEFINED__

/* interface ISum */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISum;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0703F26A-B6AC-4E50-983E-275AC15D2DF0")
    ISum : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SumOfTwoIntegers( 
            /* [in] */ int __MIDL__ISum0000,
            /* [in] */ int __MIDL__ISum0001,
            /* [out][in] */ int *__MIDL__ISum0002) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISumVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISum * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISum * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISum * This);
        
        HRESULT ( STDMETHODCALLTYPE *SumOfTwoIntegers )( 
            ISum * This,
            /* [in] */ int __MIDL__ISum0000,
            /* [in] */ int __MIDL__ISum0001,
            /* [out][in] */ int *__MIDL__ISum0002);
        
        END_INTERFACE
    } ISumVtbl;

    interface ISum
    {
        CONST_VTBL struct ISumVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISum_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISum_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISum_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISum_SumOfTwoIntegers(This,__MIDL__ISum0000,__MIDL__ISum0001,__MIDL__ISum0002)	\
    ( (This)->lpVtbl -> SumOfTwoIntegers(This,__MIDL__ISum0000,__MIDL__ISum0001,__MIDL__ISum0002) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISum_INTERFACE_DEFINED__ */


#ifndef __ISubtract_INTERFACE_DEFINED__
#define __ISubtract_INTERFACE_DEFINED__

/* interface ISubtract */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISubtract;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("501D0F4A-8303-4174-90A8-4CCEC057CFE9")
    ISubtract : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubtractionOfTwoIntegers( 
            /* [in] */ int __MIDL__ISubtract0000,
            /* [in] */ int __MIDL__ISubtract0001,
            /* [out][in] */ int *__MIDL__ISubtract0002) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISubtractVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISubtract * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISubtract * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISubtract * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubtractionOfTwoIntegers )( 
            ISubtract * This,
            /* [in] */ int __MIDL__ISubtract0000,
            /* [in] */ int __MIDL__ISubtract0001,
            /* [out][in] */ int *__MIDL__ISubtract0002);
        
        END_INTERFACE
    } ISubtractVtbl;

    interface ISubtract
    {
        CONST_VTBL struct ISubtractVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISubtract_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISubtract_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISubtract_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISubtract_SubtractionOfTwoIntegers(This,__MIDL__ISubtract0000,__MIDL__ISubtract0001,__MIDL__ISubtract0002)	\
    ( (This)->lpVtbl -> SubtractionOfTwoIntegers(This,__MIDL__ISubtract0000,__MIDL__ISubtract0001,__MIDL__ISubtract0002) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISubtract_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


