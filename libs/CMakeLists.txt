set( THREADS_PTHREAD_ARG 
     "0-FAILED_TO_RUN"
     CACHE STRING "Result from TRY_RUN" FORCE)
add_subdirectory(vendor/tdlib)
add_subdirectory(qtdlib)
add_subdirectory(quickflux)
