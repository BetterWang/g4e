target_include_directories(g4e PUBLIC ./ )

add_subdirectory(ffi_ZDC)
add_subdirectory(ci_TRD)
add_subdirectory(ce_EMCAL)
add_subdirectory(ir_Beampipe)
add_subdirectory(ci_DRICH)
# add_subdirectory(ffe_LUMI/lumi)
# target_link_library(g4e det)

