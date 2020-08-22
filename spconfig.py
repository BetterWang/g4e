#!/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/bin/python3
#

import sys
import os
import subprocess

def cmdlist(str):
    return list(x.strip().replace("'",'') for x in str.split('\n') if x)
env = dict(os.environ)
env['CC'] = '/usr/bin/gcc-7'
env['CMAKE_PREFIX_PATH'] = ":".join(cmdlist("""
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xxhash-0.7.4-bjkkmw3sml6hgafhbs4iu6asxt6vzawi
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libtiff-4.0.10-scsxpvq77amykzmqvkgnk5tgdlfqohwu
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-0.4.0-wpypiugrudp65zjbykvlpegmqe5sjn6e
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrandr-1.5.0-vtwnaqgoo2y7r2deljiuc27i26p6bg72
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gl2ps-1.4.0-so5jqxryyeh3ye2x3s7seqtunmwsaddh
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/fontconfig-2.13.1-gz35rrdrth4soxsednnv7uudsxvjyjxd
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xrandr-1.5.0-tkmrogxmdl4tbjzmoaxq3v4yynubd6an
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openblas-0.3.10-pqbigaplt5kum5jmaybrxpizny7e7hui
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia6-6.4.28-gov6ce4udw3f2e7jh5k42pj2ohpy567c
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4particlexs-2.1-w77chhiqsgiyokdtxkhww3lj6k6srwub
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xz-5.2.5-ap2ytwstahewbfe3pudjnjq75kskr6sf
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gettext-0.20.2-cnuhmkfcvgoesgwj26di4k4lg763bspx
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/double-conversion-2.0.1-etzz5bv54nrmhzaws7ayv2glvypsyx5k
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/inputproto-2.3.2-nygmrir4z3yqgtwcio2fmjc3vaasrelk
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/tar-1.32-gbcnhve54i6xrlt64a52wlstjmgnhbyi
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libffi-3.3-brymptobnuewugxs3nsp4fstq2l3utgb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libice-1.0.9-zu5tj6op7f2b2grfnnfvd25coww5cxmj
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glew-2.0.0-427deyyyjkynxp6ywj737uksndyolctf
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre-8.44-cwyktoyumpipwdykmqsicjbtzdh35n3m
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdamage-1.1.4-e44zhsrtunbpcfcr2im6ylvhqb7mi445
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxt-1.1.5-tubkfx3e6j6bfxnemi5o7d2al4225q44
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xproto-7.0.31-ftyljli2k4qefy7frrrrthhflgtqotly
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libedit-3.1-20191231-5nifbenft4lrmvuuju7rrjqidlq4tcgv
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4incl-1.0-empoqp7a2pkrn4cr3uiumhvrs55nmjsy
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-18.3.6-izdjrh6hszebog67nqsydumgkki2z74e
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-image-0.4.0-ewqb6itsjsydjj5xcozl3ahbv5itw35q
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libbsd-0.10.0-4sjkeqtlmeopo757drzoyhmpgrqvhic6
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/harfbuzz-2.6.8-2bqilporsi5qcgtxtb6mjgfeogxj22fc
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxxf86vm-1.1.4-ehgxpf277qaxh3ku5ccakfqf5vftjt36
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/readline-8.0-4vxgclcoev7s22lxj3rx5xx3m6w3iffh
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-keysyms-0.4.0-l2j6ntheb6y4spvabjkcwa3mvh6qgsky
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/expat-2.2.9-xfxakgeu24sflmnhquplnmc5bgsi4yng
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freetype-2.10.1-qisfoc3prmaz2sydblyds2f27vahi2yg
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libx11-1.6.7-2s3ghjzz3euoebjqbbv6omoanohoykir
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zlib-1.2.11-egwcgg2m23u6kiqdrywuxn5tansov3pp
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/randrproto-1.5.0-skwjo3s57cj7hog3yp2u73zvi4gr7kyz
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zstd-1.4.5-ngae2yousnlxao44h6z54m6lnirldzh5
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openssl-1.1.1g-u7dicovcgylqfqvcslhmdgnq7hgryya7
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxmu-1.1.2-uixdes626v3d5dx2kfarsx56pcoaybmj
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ncurses-6.2-emhiiqvj3auacuw6pebxdpxckml47zed
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/clhep-2.4.1.3-kckyprcr3qf6rdsvouiwdffqi3is275x
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/rngstreams-1.0.1-gjxzmim5f73iggbmfe4hlxhaqizmpybd
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xextproto-7.3.0-4chn2haj33cxficya4cuussmnajjwhl6
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxau-1.0.8-ngoxun36tkd56lkbouys55ipwmfzdn3p
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libmng-2.0.3-ljamoovp3jfa5d7mq2cytuuk2gwbwzld
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-wm-0.4.1-wd2mmmf4mvwpmdmwpqmqchopmivu4b6u
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpthread-stubs-0.4-n7pprdga3iiuxtnmxon55kenoubyoaqh
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xerces-c-3.2.2-2qzwlaudxk4wtodnvbvzy63l4wvirdht
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/unuran-1.8.1-bibzfovr35ypxg55db5edbn4lpvum6um
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vdt-0.4.3-4klw46thnkwwidgwy6spqemhnzmkcisc
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glib-2.64.3-6sdd5rdfvpdq47qgkeqazekmq2j3loxz
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-glu-9.0.1-lzbcm76kvdqfnnclozmyjsmipq7pfddr
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libjpeg-turbo-2.0.4-72isbcrpw52gavbkn5ek2f2n36strpgl
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4ndl-4.6-pkotpzdghzj7d7gxmx3xeotujsc2tkjw
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4realsurface-2.1.1-d4ixu57hljcb4cgbwindtllzx52iqanl
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/boost-1.73.0-7ru5p6h3ww5ahfnktjhqxl3fvtbbba3b
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cairo-1.16.0-2mbdizggja2miwzfydzucgcur56sq56i
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4pii-1.3-cdfyxbndwl5bqnrp3pppyaon5c3v2or7
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/sqlite-3.31.1-72r5wqvwn4ohwypripvpqj46hvlplarp
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/swig-4.0.1-44pwng4ignogsw3phxhxbo4udknp6skm
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-data-10.6.2-sdyyd6bvn7zg6m5f46hvetyejvm2ankq
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/intel-tbb-2020.2-c5vxqsish3ulfxppogmijqugpj5xn6mq
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lcms-2.9-ueqtivtbjam22fbp6zkkzspsfv2dv725
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxext-1.3.3-cobzwg2dxw6jyloaksrazqz3egshp3hy
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/util-macros-1.19.1-qrombvharzletuvomwyfwf7ie3hdyghz
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-renderutil-0.3.9-eazafgvzmqd4y23t65zzs6mtexfsyf7y
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre2-10.35-3hvj2lwm5vrm7yo4ajfumzdhuw6gthgq
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libuuid-1.0.3-nnnohlxcyjmgme5akoroekr35ny2oqfp
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libsm-1.2.2-op7brx7o6c67karnfzumynz7eamkgiz5
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia8-8244-uvwuhsp6ippnpyv3hj757amwl32eqn4p
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gsl-2.5-u2zasao63spt5m6hwhvevmomhcjws4n5
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ftgl-2.4.0-sqt2e7kq3s537uihkx7zflq6ctyfosvk
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hwloc-2.2.0-3l2q4thrcfycll72ahut4vcclrwjhfgj
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdmcp-1.1.2-glxf63wpa7ez7ac27yrsz4pobue4jypl
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/qt-5.14.2-b3jznylmadxl6ghvr6gkuzv4qlfr3nzb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxkbcommon-0.8.2-dcjd46vrdehuusitsnsulwqeftuc5opl
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4photonevaporation-5.5-rrupsgzule5h7h5hkwzcodtis2se2ugw
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4radioactivedecay-5.4-mr6fffziixqvmr5s2kx2vd7a2wqdyqko
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/font-util-1.3.2-33galwcgz7jp6zhwilcqfxvm6ive4zkr
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xkbdata-1.0.1-jq7tw3qstxde5iydhscndqdvpy7zrinp
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/binutils-2.34-jqfxqbzinhshrvg5zgb5iixyb2jcnhqx
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxpm-3.5.12-xdhmg44smz2qmeux4i4ru3jabvhxp6ud
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/llvm-10.0.0-4lrv6mowiiwmdcei65ri5m74ldvluixb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxml2-2.9.10-6necla756wpidioyad35jsy5hk2f7a3v
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4ensdfstate-2.2-yhkk55oollfc2sysqujkyqqdsq4zhiqb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/davix-0.7.5-xbhw7zhffkoik4tlkiu67kq3padoucrz
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxfixes-5.0.2-gfuoe72aqvwvjdbvlbeietisjzidr3j7
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hepmc-2.06.10-rwumsvbky7daocgqabgxgbe63k2x2wtb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpng-1.6.37-uwmc5rsorglviiam52oxuquowlwexl46
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cmake-3.18.0-p3b6iof4r2suyqxikkghqpjwdn5mhtqx
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxft-2.3.2-t46kqsnco2xhlnrvrsq53yll7yu5wbse
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lz4-1.9.2-od5mcwixehcb3xpp4jhsys52thlz7ufk
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/bzip2-1.0.8-ytooyqlr5dyd6htdd2gcqmyg65m3v2x7
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/kbproto-1.0.7-qi6dnlqkaue26fm7kwwghhyd6dcyldoc
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freeglut-3.0.0-jyb7apva3inxdbveuge7lo4miyvznfa5
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4abla-3.1-snnnzc5jljpkurjxf23ktgotjiompya3
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxi-1.7.6-fzykebjekkhmgivjarumr442sqtbodxi
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gdbm-1.18.1-35ah4rucwu7rcliu7dsaovavha36sg2p
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/renderproto-0.11.1-e6wkovqbv22ybwc7nbfkjhotehxwlra7
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxcb-1.13-xuuq2ctxvboveqmfgdfwvjm545z4b7uk
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pixman-0.40.0-j63frfjmwq7pzlh3albuejqbonmefcr5
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4emlow-7.9.1-66py2oixzrxnhqbmeo5m37qofua33pmd
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/z3-4.8.7-vawiaxahik7r2gn4zffufnpu6nlylgyj
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/icu4c-67.1-5fxigdz7kgbdezxnpdrbkgziikcwskcp
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4saiddata-2.0-eskhoyjtx6jfp627yssqz246hz2nemkh
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpciaccess-0.13.5-snnubsokkhl6zrxbydh4v6ykpqtl65bb
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrender-0.9.10-3odtwskbmu3q7h5leshuvtgt2lgledsd
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libdrm-2.4.100-kdtdgdg5uyuvectomzmmnwdf3grpsqbk
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libiconv-1.16-gt23fculodq7k3ugkfnf43m3bfe6ap6i
"""))
env['CXX'] = '/usr/bin/g++-7'
env['FC'] = '/usr/bin/gfortran-7'
env['PATH'] = ":".join(cmdlist("""
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/py-numpy-1.19.1-7lzuqv75qp52ntrgge7wf5qtjatnlqc7/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cmake-3.18.0-p3b6iof4r2suyqxikkghqpjwdn5mhtqx/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cmake-3.18.0-p3b6iof4r2suyqxikkghqpjwdn5mhtqx/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4e-local-wxrsjsl5ldxlmlfewkcjhefwfi6ec64w/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zstd-1.4.5-ngae2yousnlxao44h6z54m6lnirldzh5/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xxhash-0.7.4-bjkkmw3sml6hgafhbs4iu6asxt6vzawi/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia8-8244-uvwuhsp6ippnpyv3hj757amwl32eqn4p/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/py-numpy-1.19.1-7lzuqv75qp52ntrgge7wf5qtjatnlqc7/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openblas-0.3.10-pqbigaplt5kum5jmaybrxpizny7e7hui/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lz4-1.9.2-od5mcwixehcb3xpp4jhsys52thlz7ufk/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxpm-3.5.12-xdhmg44smz2qmeux4i4ru3jabvhxp6ud/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gsl-2.5-u2zasao63spt5m6hwhvevmomhcjws4n5/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glew-2.0.0-427deyyyjkynxp6ywj737uksndyolctf/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xrandr-1.5.0-tkmrogxmdl4tbjzmoaxq3v4yynubd6an/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/davix-0.7.5-xbhw7zhffkoik4tlkiu67kq3padoucrz/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xerces-c-3.2.2-2qzwlaudxk4wtodnvbvzy63l4wvirdht/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/qt-5.14.2-b3jznylmadxl6ghvr6gkuzv4qlfr3nzb/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre2-10.35-3hvj2lwm5vrm7yo4ajfumzdhuw6gthgq/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lcms-2.9-ueqtivtbjam22fbp6zkkzspsfv2dv725/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libtiff-4.0.10-scsxpvq77amykzmqvkgnk5tgdlfqohwu/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libjpeg-turbo-2.0.4-72isbcrpw52gavbkn5ek2f2n36strpgl/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/harfbuzz-2.6.8-2bqilporsi5qcgtxtb6mjgfeogxj22fc/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/icu4c-67.1-5fxigdz7kgbdezxnpdrbkgziikcwskcp/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cairo-1.16.0-2mbdizggja2miwzfydzucgcur56sq56i/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glib-2.64.3-6sdd5rdfvpdq47qgkeqazekmq2j3loxz/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/perl-5.30.3-u3gx4ys5i7iwqhbylxdnd2hetjg7nn6u/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/fontconfig-2.13.1-gz35rrdrth4soxsednnv7uudsxvjyjxd/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freetype-2.10.1-qisfoc3prmaz2sydblyds2f27vahi2yg/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpng-1.6.37-uwmc5rsorglviiam52oxuquowlwexl46/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/font-util-1.3.2-33galwcgz7jp6zhwilcqfxvm6ive4zkr/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/llvm-10.0.0-4lrv6mowiiwmdcei65ri5m74ldvluixb/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/z3-4.8.7-vawiaxahik7r2gn4zffufnpu6nlylgyj/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/py-setuptools-46.1.3-cy7n3trwrik5hwcmwhgitsm3aug5mygp/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/swig-4.0.1-44pwng4ignogsw3phxhxbo4udknp6skm/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre-8.44-cwyktoyumpipwdykmqsicjbtzdh35n3m/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hwloc-2.2.0-3l2q4thrcfycll72ahut4vcclrwjhfgj/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/binutils-2.34-jqfxqbzinhshrvg5zgb5iixyb2jcnhqx/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/clhep-2.4.1.3-kckyprcr3qf6rdsvouiwdffqi3is275x/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/sqlite-3.31.1-72r5wqvwn4ohwypripvpqj46hvlplarp/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openssl-1.1.1g-u7dicovcgylqfqvcslhmdgnq7hgryya7/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gettext-0.20.2-cnuhmkfcvgoesgwj26di4k4lg763bspx/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/tar-1.32-gbcnhve54i6xrlt64a52wlstjmgnhbyi/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxml2-2.9.10-6necla756wpidioyad35jsy5hk2f7a3v/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xz-5.2.5-ap2ytwstahewbfe3pudjnjq75kskr6sf/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libiconv-1.16-gt23fculodq7k3ugkfnf43m3bfe6ap6i/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gdbm-1.18.1-35ah4rucwu7rcliu7dsaovavha36sg2p/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/readline-8.0-4vxgclcoev7s22lxj3rx5xx3m6w3iffh/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ncurses-6.2-emhiiqvj3auacuw6pebxdpxckml47zed/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/expat-2.2.9-xfxakgeu24sflmnhquplnmc5bgsi4yng/bin
    /home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/bzip2-1.0.8-ytooyqlr5dyd6htdd2gcqmyg65m3v2x7/bin
    /home/romanov/jlab/ejpm/bin
    /home/romanov/.local/bin
    /home/romanov/.vscode-server/bin/db40434f562994116e5b21c24015a2e40b2504e6/bin
    /home/romanov/.local/bin
    /home/romanov/jlab/ejpm/bin
    /home/romanov/.local/bin
    /home/romanov/eic/spack/bin
    /usr/local/sbin
    /usr/local/bin
    /usr/sbin
    /usr/bin
    /sbin
    /bin
    /usr/games
    /usr/local/games
    /mnt/c/Windows/system32
    /mnt/c/Windows
    /mnt/c/Windows/System32/Wbem
    /mnt/c/Windows/System32/WindowsPowerShell/v1.0/
    /mnt/c/Windows/System32/OpenSSH/
    /mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common
    /mnt/c/Program Files/Microsoft SQL Server/130/Tools/Binn/
    /mnt/c/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/Tools/Binn/
    /mnt/c/Program Files/Intel/WiFi/bin/
    /mnt/c/Program Files/Common Files/Intel/WirelessCommon/
    /mnt/c/Program Files (x86)/ZeroTier/One/
    /mnt/c/WINDOWS/system32
    /mnt/c/WINDOWS
    /mnt/c/WINDOWS/System32/Wbem
    /mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/
    /mnt/c/WINDOWS/System32/OpenSSH/
    /mnt/c/Program Files/Microsoft SQL Server/120/Tools/Binn/
    /mnt/c/Program Files/dotnet/
    /mnt/c/Program Files/Git/cmd
    /mnt/c/Program Files/PuTTY/
    /mnt/c/Program Files/PowerShell/7/
    /mnt/c/Program Files/Docker/Docker/resources/bin
    /mnt/c/ProgramData/DockerDesktop/version-bin
    /mnt/c/Users/romanov/AppData/Local/Microsoft/WindowsApps
    /mnt/c/Users/romanov/AppData/Local/Programs/Microsoft VS Code/bin
    /mnt/c/Program Files/Intel/WiFi/bin/
    /mnt/c/Program Files/Common Files/Intel/WirelessCommon/
    /mnt/c/Users/romanov/.dotnet/tools
    /mnt/c/Users/romanov/AppData/Local/Microsoft/WindowsApps
    /mnt/c/Users/romanov/.dotnet/tools
    /snap/bin
"""))
env['SPACK_INCLUDE_DIRS'] = '/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xxhash-0.7.4-bjkkmw3sml6hgafhbs4iu6asxt6vzawi/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gl2ps-1.4.0-so5jqxryyeh3ye2x3s7seqtunmwsaddh/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/fontconfig-2.13.1-gz35rrdrth4soxsednnv7uudsxvjyjxd/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openblas-0.3.10-pqbigaplt5kum5jmaybrxpizny7e7hui/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xz-5.2.5-ap2ytwstahewbfe3pudjnjq75kskr6sf/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/include/python3.7m:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gettext-0.20.2-cnuhmkfcvgoesgwj26di4k4lg763bspx/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/double-conversion-2.0.1-etzz5bv54nrmhzaws7ayv2glvypsyx5k/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/inputproto-2.3.2-nygmrir4z3yqgtwcio2fmjc3vaasrelk/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libice-1.0.9-zu5tj6op7f2b2grfnnfvd25coww5cxmj/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre-8.44-cwyktoyumpipwdykmqsicjbtzdh35n3m/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdamage-1.1.4-e44zhsrtunbpcfcr2im6ylvhqb7mi445/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxt-1.1.5-tubkfx3e6j6bfxnemi5o7d2al4225q44/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libedit-3.1-20191231-5nifbenft4lrmvuuju7rrjqidlq4tcgv/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/readline-8.0-4vxgclcoev7s22lxj3rx5xx3m6w3iffh/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-keysyms-0.4.0-l2j6ntheb6y4spvabjkcwa3mvh6qgsky/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freetype-2.10.1-qisfoc3prmaz2sydblyds2f27vahi2yg/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zlib-1.2.11-egwcgg2m23u6kiqdrywuxn5tansov3pp/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/randrproto-1.5.0-skwjo3s57cj7hog3yp2u73zvi4gr7kyz/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxmu-1.1.2-uixdes626v3d5dx2kfarsx56pcoaybmj/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/clhep-2.4.1.3-kckyprcr3qf6rdsvouiwdffqi3is275x/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/rngstreams-1.0.1-gjxzmim5f73iggbmfe4hlxhaqizmpybd/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xextproto-7.3.0-4chn2haj33cxficya4cuussmnajjwhl6/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libmng-2.0.3-ljamoovp3jfa5d7mq2cytuuk2gwbwzld/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/unuran-1.8.1-bibzfovr35ypxg55db5edbn4lpvum6um/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vdt-0.4.3-4klw46thnkwwidgwy6spqemhnzmkcisc/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glib-2.64.3-6sdd5rdfvpdq47qgkeqazekmq2j3loxz/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/boost-1.73.0-7ru5p6h3ww5ahfnktjhqxl3fvtbbba3b/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/sqlite-3.31.1-72r5wqvwn4ohwypripvpqj46hvlplarp/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lcms-2.9-ueqtivtbjam22fbp6zkkzspsfv2dv725/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxext-1.3.3-cobzwg2dxw6jyloaksrazqz3egshp3hy/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre2-10.35-3hvj2lwm5vrm7yo4ajfumzdhuw6gthgq/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libuuid-1.0.3-nnnohlxcyjmgme5akoroekr35ny2oqfp/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libsm-1.2.2-op7brx7o6c67karnfzumynz7eamkgiz5/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia8-8244-uvwuhsp6ippnpyv3hj757amwl32eqn4p/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ftgl-2.4.0-sqt2e7kq3s537uihkx7zflq6ctyfosvk/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/llvm-10.0.0-4lrv6mowiiwmdcei65ri5m74ldvluixb/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpng-1.6.37-uwmc5rsorglviiam52oxuquowlwexl46/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxft-2.3.2-t46kqsnco2xhlnrvrsq53yll7yu5wbse/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lz4-1.9.2-od5mcwixehcb3xpp4jhsys52thlz7ufk/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/bzip2-1.0.8-ytooyqlr5dyd6htdd2gcqmyg65m3v2x7/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freeglut-3.0.0-jyb7apva3inxdbveuge7lo4miyvznfa5/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/renderproto-0.11.1-e6wkovqbv22ybwc7nbfkjhotehxwlra7/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/icu4c-67.1-5fxigdz7kgbdezxnpdrbkgziikcwskcp/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpciaccess-0.13.5-snnubsokkhl6zrxbydh4v6ykpqtl65bb/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libx11-1.6.7-2s3ghjzz3euoebjqbbv6omoanohoykir/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrender-0.9.10-3odtwskbmu3q7h5leshuvtgt2lgledsd/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libdrm-2.4.100-kdtdgdg5uyuvectomzmmnwdf3grpsqbk/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libtiff-4.0.10-scsxpvq77amykzmqvkgnk5tgdlfqohwu/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-0.4.0-wpypiugrudp65zjbykvlpegmqe5sjn6e/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrandr-1.5.0-vtwnaqgoo2y7r2deljiuc27i26p6bg72/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libffi-3.3-brymptobnuewugxs3nsp4fstq2l3utgb/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glew-2.0.0-427deyyyjkynxp6ywj737uksndyolctf/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xproto-7.0.31-ftyljli2k4qefy7frrrrthhflgtqotly/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-18.3.6-izdjrh6hszebog67nqsydumgkki2z74e/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-image-0.4.0-ewqb6itsjsydjj5xcozl3ahbv5itw35q/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libbsd-0.10.0-4sjkeqtlmeopo757drzoyhmpgrqvhic6/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/harfbuzz-2.6.8-2bqilporsi5qcgtxtb6mjgfeogxj22fc/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxxf86vm-1.1.4-ehgxpf277qaxh3ku5ccakfqf5vftjt36/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/expat-2.2.9-xfxakgeu24sflmnhquplnmc5bgsi4yng/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zstd-1.4.5-ngae2yousnlxao44h6z54m6lnirldzh5/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openssl-1.1.1g-u7dicovcgylqfqvcslhmdgnq7hgryya7/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ncurses-6.2-emhiiqvj3auacuw6pebxdpxckml47zed/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxau-1.0.8-ngoxun36tkd56lkbouys55ipwmfzdn3p/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-wm-0.4.1-wd2mmmf4mvwpmdmwpqmqchopmivu4b6u/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xerces-c-3.2.2-2qzwlaudxk4wtodnvbvzy63l4wvirdht/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-glu-9.0.1-lzbcm76kvdqfnnclozmyjsmipq7pfddr/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libjpeg-turbo-2.0.4-72isbcrpw52gavbkn5ek2f2n36strpgl/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cairo-1.16.0-2mbdizggja2miwzfydzucgcur56sq56i/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/intel-tbb-2020.2-c5vxqsish3ulfxppogmijqugpj5xn6mq/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-renderutil-0.3.9-eazafgvzmqd4y23t65zzs6mtexfsyf7y/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gsl-2.5-u2zasao63spt5m6hwhvevmomhcjws4n5/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hwloc-2.2.0-3l2q4thrcfycll72ahut4vcclrwjhfgj/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdmcp-1.1.2-glxf63wpa7ez7ac27yrsz4pobue4jypl/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/qt-5.14.2-b3jznylmadxl6ghvr6gkuzv4qlfr3nzb/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxkbcommon-0.8.2-dcjd46vrdehuusitsnsulwqeftuc5opl/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/binutils-2.34-jqfxqbzinhshrvg5zgb5iixyb2jcnhqx/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxpm-3.5.12-xdhmg44smz2qmeux4i4ru3jabvhxp6ud/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxml2-2.9.10-6necla756wpidioyad35jsy5hk2f7a3v/include/libxml2:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/davix-0.7.5-xbhw7zhffkoik4tlkiu67kq3padoucrz/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxfixes-5.0.2-gfuoe72aqvwvjdbvlbeietisjzidr3j7/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hepmc-2.06.10-rwumsvbky7daocgqabgxgbe63k2x2wtb/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/kbproto-1.0.7-qi6dnlqkaue26fm7kwwghhyd6dcyldoc/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxi-1.7.6-fzykebjekkhmgivjarumr442sqtbodxi/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gdbm-1.18.1-35ah4rucwu7rcliu7dsaovavha36sg2p/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxcb-1.13-xuuq2ctxvboveqmfgdfwvjm545z4b7uk/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pixman-0.40.0-j63frfjmwq7pzlh3albuejqbonmefcr5/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/z3-4.8.7-vawiaxahik7r2gn4zffufnpu6nlylgyj/include:/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libiconv-1.16-gt23fculodq7k3ugkfnf43m3bfe6ap6i/include'

cmd = cmdlist("""
/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cmake-3.18.0-p3b6iof4r2suyqxikkghqpjwdn5mhtqx/bin/cmake
    -G
    Unix Makefiles
    -DCMAKE_INSTALL_PREFIX:STRING=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4e-local-wxrsjsl5ldxlmlfewkcjhefwfi6ec64w
    -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=OFF
    -DCMAKE_INSTALL_RPATH:STRING=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4e-local-wxrsjsl5ldxlmlfewkcjhefwfi6ec64w/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/g4e-local-wxrsjsl5ldxlmlfewkcjhefwfi6ec64w/lib64;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/boost-1.73.0-7ru5p6h3ww5ahfnktjhqxl3fvtbbba3b/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/bzip2-1.0.8-ytooyqlr5dyd6htdd2gcqmyg65m3v2x7/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/python-3.7.7-s2dyowndkk7nwhztowt2ksrlujgwzos4/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/expat-2.2.9-xfxakgeu24sflmnhquplnmc5bgsi4yng/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libbsd-0.10.0-4sjkeqtlmeopo757drzoyhmpgrqvhic6/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gdbm-1.18.1-35ah4rucwu7rcliu7dsaovavha36sg2p/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/readline-8.0-4vxgclcoev7s22lxj3rx5xx3m6w3iffh/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ncurses-6.2-emhiiqvj3auacuw6pebxdpxckml47zed/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gettext-0.20.2-cnuhmkfcvgoesgwj26di4k4lg763bspx/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libiconv-1.16-gt23fculodq7k3ugkfnf43m3bfe6ap6i/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxml2-2.9.10-6necla756wpidioyad35jsy5hk2f7a3v/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xz-5.2.5-ap2ytwstahewbfe3pudjnjq75kskr6sf/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zlib-1.2.11-egwcgg2m23u6kiqdrywuxn5tansov3pp/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libffi-3.3-brymptobnuewugxs3nsp4fstq2l3utgb/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openssl-1.1.1g-u7dicovcgylqfqvcslhmdgnq7hgryya7/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/sqlite-3.31.1-72r5wqvwn4ohwypripvpqj46hvlplarp/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/clhep-2.4.1.3-kckyprcr3qf6rdsvouiwdffqi3is275x/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-18.3.6-izdjrh6hszebog67nqsydumgkki2z74e/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libx11-1.6.7-2s3ghjzz3euoebjqbbv6omoanohoykir/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/kbproto-1.0.7-qi6dnlqkaue26fm7kwwghhyd6dcyldoc/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxcb-1.13-xuuq2ctxvboveqmfgdfwvjm545z4b7uk/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpthread-stubs-0.4-n7pprdga3iiuxtnmxon55kenoubyoaqh/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxau-1.0.8-ngoxun36tkd56lkbouys55ipwmfzdn3p/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xproto-7.0.31-ftyljli2k4qefy7frrrrthhflgtqotly/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdmcp-1.1.2-glxf63wpa7ez7ac27yrsz4pobue4jypl/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xextproto-7.3.0-4chn2haj33cxficya4cuussmnajjwhl6/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxext-1.3.3-cobzwg2dxw6jyloaksrazqz3egshp3hy/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/llvm-10.0.0-4lrv6mowiiwmdcei65ri5m74ldvluixb/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/binutils-2.34-jqfxqbzinhshrvg5zgb5iixyb2jcnhqx/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hwloc-2.2.0-3l2q4thrcfycll72ahut4vcclrwjhfgj/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpciaccess-0.13.5-snnubsokkhl6zrxbydh4v6ykpqtl65bb/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libedit-3.1-20191231-5nifbenft4lrmvuuju7rrjqidlq4tcgv/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre-8.44-cwyktoyumpipwdykmqsicjbtzdh35n3m/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/z3-4.8.7-vawiaxahik7r2gn4zffufnpu6nlylgyj/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/qt-5.14.2-b3jznylmadxl6ghvr6gkuzv4qlfr3nzb/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/double-conversion-2.0.1-etzz5bv54nrmhzaws7ayv2glvypsyx5k/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/fontconfig-2.13.1-gz35rrdrth4soxsednnv7uudsxvjyjxd/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/font-util-1.3.2-33galwcgz7jp6zhwilcqfxvm6ive4zkr/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freetype-2.10.1-qisfoc3prmaz2sydblyds2f27vahi2yg/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libpng-1.6.37-uwmc5rsorglviiam52oxuquowlwexl46/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libuuid-1.0.3-nnnohlxcyjmgme5akoroekr35ny2oqfp/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glib-2.64.3-6sdd5rdfvpdq47qgkeqazekmq2j3loxz/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/harfbuzz-2.6.8-2bqilporsi5qcgtxtb6mjgfeogxj22fc/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cairo-1.16.0-2mbdizggja2miwzfydzucgcur56sq56i/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pixman-0.40.0-j63frfjmwq7pzlh3albuejqbonmefcr5/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/icu4c-67.1-5fxigdz7kgbdezxnpdrbkgziikcwskcp/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libjpeg-turbo-2.0.4-72isbcrpw52gavbkn5ek2f2n36strpgl/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lcms-2.9-ueqtivtbjam22fbp6zkkzspsfv2dv725/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libtiff-4.0.10-scsxpvq77amykzmqvkgnk5tgdlfqohwu/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libsm-1.2.2-op7brx7o6c67karnfzumynz7eamkgiz5/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libice-1.0.9-zu5tj6op7f2b2grfnnfvd25coww5cxmj/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxkbcommon-0.8.2-dcjd46vrdehuusitsnsulwqeftuc5opl/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrender-0.9.10-3odtwskbmu3q7h5leshuvtgt2lgledsd/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/renderproto-0.11.1-e6wkovqbv22ybwc7nbfkjhotehxwlra7/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pcre2-10.35-3hvj2lwm5vrm7yo4ajfumzdhuw6gthgq/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-image-0.4.0-ewqb6itsjsydjj5xcozl3ahbv5itw35q/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-0.4.0-wpypiugrudp65zjbykvlpegmqe5sjn6e/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-keysyms-0.4.0-l2j6ntheb6y4spvabjkcwa3mvh6qgsky/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-renderutil-0.3.9-eazafgvzmqd4y23t65zzs6mtexfsyf7y/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xcb-util-wm-0.4.1-wd2mmmf4mvwpmdmwpqmqchopmivu4b6u/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xerces-c-3.2.2-2qzwlaudxk4wtodnvbvzy63l4wvirdht/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hepmc-2.06.10-rwumsvbky7daocgqabgxgbe63k2x2wtb/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/davix-0.7.5-xbhw7zhffkoik4tlkiu67kq3padoucrz/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/ftgl-2.4.0-sqt2e7kq3s537uihkx7zflq6ctyfosvk/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/mesa-glu-9.0.1-lzbcm76kvdqfnnclozmyjsmipq7pfddr/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gl2ps-1.4.0-so5jqxryyeh3ye2x3s7seqtunmwsaddh/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/freeglut-3.0.0-jyb7apva3inxdbveuge7lo4miyvznfa5/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/inputproto-2.3.2-nygmrir4z3yqgtwcio2fmjc3vaasrelk/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxi-1.7.6-fzykebjekkhmgivjarumr442sqtbodxi/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxfixes-5.0.2-gfuoe72aqvwvjdbvlbeietisjzidr3j7/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxrandr-1.5.0-vtwnaqgoo2y7r2deljiuc27i26p6bg72/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/randrproto-1.5.0-skwjo3s57cj7hog3yp2u73zvi4gr7kyz/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libdrm-2.4.100-kdtdgdg5uyuvectomzmmnwdf3grpsqbk/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxdamage-1.1.4-e44zhsrtunbpcfcr2im6ylvhqb7mi445/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxmu-1.1.2-uixdes626v3d5dx2kfarsx56pcoaybmj/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxt-1.1.5-tubkfx3e6j6bfxnemi5o7d2al4225q44/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxxf86vm-1.1.4-ehgxpf277qaxh3ku5ccakfqf5vftjt36/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/glew-2.0.0-427deyyyjkynxp6ywj737uksndyolctf/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/gsl-2.5-u2zasao63spt5m6hwhvevmomhcjws4n5/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/intel-tbb-2020.2-c5vxqsish3ulfxppogmijqugpj5xn6mq/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxft-2.3.2-t46kqsnco2xhlnrvrsq53yll7yu5wbse/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libxpm-3.5.12-xdhmg44smz2qmeux4i4ru3jabvhxp6ud/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/lz4-1.9.2-od5mcwixehcb3xpp4jhsys52thlz7ufk/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/openblas-0.3.10-pqbigaplt5kum5jmaybrxpizny7e7hui/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia6-6.4.28-gov6ce4udw3f2e7jh5k42pj2ohpy567c/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/pythia8-8244-uvwuhsp6ippnpyv3hj757amwl32eqn4p/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/unuran-1.8.1-bibzfovr35ypxg55db5edbn4lpvum6um/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/rngstreams-1.0.1-gjxzmim5f73iggbmfe4hlxhaqizmpybd/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vdt-0.4.3-4klw46thnkwwidgwy6spqemhnzmkcisc/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/xxhash-0.7.4-bjkkmw3sml6hgafhbs4iu6asxt6vzawi/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/zstd-1.4.5-ngae2yousnlxao44h6z54m6lnirldzh5/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn/lib;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/libmng-2.0.3-ljamoovp3jfa5d7mq2cytuuk2gwbwzld/lib64
    -DCMAKE_PREFIX_PATH:STRING=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/cmake-3.18.0-p3b6iof4r2suyqxikkghqpjwdn5mhtqx;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn;/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hepmc-2.06.10-rwumsvbky7daocgqabgxgbe63k2x2wtb
    -DCMAKE_CXX_STANDARD=17
    -DGEANT4_DIR=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/geant4-10.6.2-vypw7yk46622gzkvfwezl72mmkpknhgw
    -DVGM_DIRECTORY=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/vgm-4-8-5wh2gctnegnruusfseewhbx4rzvjxngn
    -DHEPMC_DIRECTORY=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/hepmc-2.06.10-rwumsvbky7daocgqabgxgbe63k2x2wtb
    -DCERN_ROOT_DIRECTORY=/home/romanov/eic/spack/opt/spack/linux-ubuntu18.04-zen/gcc-7.5.0/root-6.20.04-75hx6iae6rwj7a2hddiirhzjgfkgk47g
""") + sys.argv[1:]

proc = subprocess.Popen(cmd, env=env)
proc.wait()
