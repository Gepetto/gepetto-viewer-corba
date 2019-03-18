#
# Copyright (c) 2014 CNRS-LAAS
# Author: Florent Lamiraux
#

HPP_REPO=https://github.com/humanoid-path-planner

DISABLE_TESTS=OFF
SRC_DIR="NOT_SET"
INSTALL_DIR="NOT_SET"

ifeq (${INSTALL_DIR}, "NOT_SET")
  $(error INSTALL_DIR not set. Please open the Makefile and set it)
endif
ifeq (${SRC_DIR},"NOT_SET")
  $(error SRC_DIR not set. Please open the Makefile and set it)
endif

BUILD_TYPE=Release
ifeq (${BUILD_TYPE},Debug)
  BUILD_FOLDER=build
else
  BUILD_FOLDER=build-rel
	DISABLE_TESTS=ON
endif

#OSG_PACKAGE=OpenSceneGraph-dae-plugin
OSG_PACKAGE=OpenSceneGraph-3.4.0

WGET=wget --quiet
UNZIP=unzip -qq
TAR=tar
GIT_QUIET=--quiet
# Qt version should be either 4 or 5
QT_VERSION=5
INSTALL_DOCUMENTATION=ON

collada-dom_branch=master
collada-dom_repository=${HPP_REPO}
collada-dom_extra_flags=-DBUILD_SHARED_LIBS=TRUE -DOPT_COLLADA15=FALSE

OpenSceneGraph-dae-plugin_branch=master
OpenSceneGraph-dae-plugin_repository=${HPP_REPO}
OpenSceneGraph-dae-plugin_extra_flags= -DCOLLADA_DYNAMIC_LIBRARY=${INSTALL_DIR}/lib/libcollada14dom.so -DCOLLADA_INCLUDE_DIR=${INSTALL_DIR}/include/collada-dom

OpenSceneGraph-3.4.0_extra_flags= -DDESIRED_QT_VERSION=${QT_VERSION} -DCOLLADA_DYNAMIC_LIBRARY=${INSTALL_DIR}/lib/libcollada14dom.so -DCOLLADA_INCLUDE_DIR=${INSTALL_DIR}/include/collada-dom -DLIB_POSTFIX=""

gepetto-viewer_branch=devel
gepetto-viewer_repository=${HPP_REPO}

gepetto-viewer-corba_branch=devel
gepetto-viewer-corba_repository=${HPP_REPO}
ifeq (${QT_VERSION}, 5)
  gepetto-viewer-corba_extra_flags= -DPROJECT_USE_QT4=OFF
endif

pythonqt_branch=qt${QT_VERSION}
pythonqt_repository=${HPP_REPO}
pythonqt_extra_flags= -DPythonQt_Wrap_QtAll=ON -DPythonQt_Extensions=ON

qgv_branch=devel
qgv_repository=${HPP_REPO}

all: gepetto-viewer-corba.install

qgv.configure.dep: qgv.checkout
pythonqt.configure.dep: pythonqt.checkout
collada-dom.configure.dep: collada-dom.checkout
OpenSceneGraph-dae-plugin.configure.dep: collada-dom.install \
	OpenSceneGraph-dae-plugin.checkout
OpenSceneGraph-3.4.0.configure.dep: collada-dom.install \
	OpenSceneGraph-3.4.0.checkout
gepetto-viewer.configure.dep: ${OSG_PACKAGE}.install \
	gepetto-viewer.checkout
gepetto-viewer-corba.configure.dep: gepetto-viewer.install \
	gepetto-viewer-corba.checkout
pythonqt.configure.dep: pythonqt.checkout

status:
	@for child_dir in $$(ls ${SRC_DIR}); do \
		test -d "$$child_dir" || continue; \
		test -d "$$child_dir/.git" || continue; \
		${MAKE} "$$child_dir".status; \
	done

log:
	@for child_dir in $$(ls ${SRC_DIR}); do \
		test -d "$$child_dir" || continue; \
		test -d "$$child_dir/.git" || continue; \
		${MAKE} "$$child_dir".log; \
	done

update:
	@for child_dir in $$(ls ${SRC_DIR}); do \
		test -d "$$child_dir" || continue; \
		test -d "$$child_dir/.git" || continue; \
		${MAKE} "$$child_dir".update; \
	done

%.checkout:
	if [ -d $(@:.checkout=) ]; then \
		echo "$(@:.checkout=) already checkout out."; \
	else \
		git clone ${GIT_QUIET} --recursive -b ${$(@:.checkout=)_branch} ${$(@:.checkout=)_repository}/$(@:.checkout=); \
	fi \

%.update:
	if [ "${$(@:.update=)_repository}" = "" ]; then \
		echo "$(@:.update=) is not referenced"; \
	else \
		cd ${SRC_DIR}/$(@:.update=);\
		git remote rm origin;\
		git remote add origin ${$(@:.update=)_repository}/$(@:.update=);\
		git fetch origin;\
		git checkout -b bce46g origin/${$(@:.update=)_branch};\
		git branch -D ${$(@:.update=)_branch};\
		git checkout -b ${$(@:.update=)_branch} bce46g;\
		git branch -D bce46g;\
		git submodule update; \
	fi


%.configure: %.configure.dep
	${MAKE} $(@:.configure=).configure_nodep

%.configure_nodep:%.checkout
	mkdir -p ${SRC_DIR}/$(@:.configure_nodep=)/${BUILD_FOLDER}; \
	cd ${SRC_DIR}/$(@:.configure_nodep=)/${BUILD_FOLDER}; \
	cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
			-DINSTALL_DOCUMENTATION=${INSTALL_DOCUMENTATION} \
			-DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-g -O3 -DNDEBUG" \
			-DDISABLE_TESTS=${DISABLE_TESTS} \
			${$(@:.configure_nodep=)_extra_flags} ..

%.install:%.configure
	${MAKE} -C ${SRC_DIR}/$(@:.install=)/${BUILD_FOLDER} install

%.install_nodep:%.configure_nodep
	${MAKE} -C ${SRC_DIR}/$(@:.install_nodep=)/${BUILD_FOLDER} install

%.uninstall:
	${MAKE} -C ${SRC_DIR}/$(@:.uninstall=)/${BUILD_FOLDER} uninstall

%.clean:
	${MAKE} -C ${SRC_DIR}/$(@:.clean=)/${BUILD_FOLDER} clean

%.very-clean:
	rm -rf ${SRC_DIR}/$(@:.very-clean=)/${BUILD_FOLDER}/*

%.status:
	@cd ${SRC_DIR}/$(@:.status=); \
	echo \
	"\033[1;36m------- Folder $(@:.status=) ---------------\033[0m"; \
	git --no-pager -c status.showUntrackedFiles=no status --short --branch;\

%.log:
	@cd ${SRC_DIR}/$(@:.log=); \
	if [ -f .git/refs/heads/${$(@:.log=)_branch} ]; then \
		echo -n "$(@:.log=): "; \
		cat .git/refs/heads/${$(@:.log=)_branch}; \
	fi

OpenSceneGraph-3.4.0.checkout:
	if [ -d $(@:.checkout=) ]; then \
		echo "$(@:.checkout=) already checkout out."; \
	else \
		wget http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.4.0/source/OpenSceneGraph-3.4.0.zip;\
		cd ${SRC_DIR}; unzip OpenSceneGraph-3.4.0.zip;\
		rm -f OpenSceneGraph-3.4.0.zip;\
	fi
