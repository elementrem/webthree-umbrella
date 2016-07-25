#!/usr/bin/env bash
#
# Script to recreate the cpp-elementrem repository by copying directories from webthree-umbrella.
#
# This is for one step of the "Dry run for repository reorganization" which is to get the content
# correct, automated and tested, before pulling the trigger on the real move, which will need to
# preserve history.
#
# See https://github.com/elementrem/webthree-umbrella/issues/453

outputDirectory=../cpp-elementrem-recreated

rm    -rf $outputDirectory
mkdir -p  $outputDirectory
mkdir -p  $outputDirectory/test/

# alelezero intentionally omitted
rsync -r ./dependency_graph/              $outputDirectory/dependency_graph/
rsync -r ./doc/                           $outputDirectory/doc/
rsync -r ./docker/                        $outputDirectory/docker/
rsync -r ./libelementrem/elekey/            $outputDirectory/elekey/
rsync -r ./libelementrem/eleminer/          $outputDirectory/eleminer/
rsync -r ./libelementrem/elevm/             $outputDirectory/elevm/
rsync -r ./libelementrem/libelhash/         $outputDirectory/libelhash/
rsync -r ./libelementrem/libelhash-cl/      $outputDirectory/libelhash-cl/
rsync -r ./libelementrem/libelhashseal/     $outputDirectory/libelhashseal/
rsync -r ./libelementrem/libelecore/        $outputDirectory/libelecore/
rsync -r ./libelementrem/libelementrem/       $outputDirectory/libelementrem/
rsync -r ./libelementrem/libevm/            $outputDirectory/libevm/
rsync -r ./libelementrem/libevmcore/        $outputDirectory/libevmcore/
rsync -r ./libelementrem/libnatspec/        $outputDirectory/libnatspec/
rsync -r ./libelementrem/libtestutils/      $outputDirectory/libtestutils/
rsync -r ./libelementrem/test/deprecated/             $outputDirectory/test/deprecated/
rsync -r ./libelementrem/test/external-dependencies/  $outputDirectory/test/external-dependencies/
rsync -r ./libelementrem/test/fuzzTesting/            $outputDirectory/test/fuzzTesting/
rsync -r ./libelementrem/test/libelecore/             $outputDirectory/test/libelecore/
rsync -r ./libelementrem/test/libelementrem/            $outputDirectory/test/libelementrem/
rsync -r ./libelementrem/test/libevm/                 $outputDirectory/test/libevm/
rsync -r ./libelementrem/test/libnatspec/             $outputDirectory/test/libnatspec/
rsync -r ./libelementrem/test/libweb3core/            $outputDirectory/test/libweb3core/
rsync -r ./libelementrem/test/BlockChainHelper.cpp    $outputDirectory/test/BlockChainHelper.cpp
rsync -r ./libelementrem/test/BlockChainHelper.h      $outputDirectory/test/BlockChainHelper.h
rsync -r ./libelementrem/test/boostTest.cpp           $outputDirectory/test/boostTest.cpp
rsync -r ./libelementrem/test/JSON_test.sol           $outputDirectory/test/JSON_test.sol
rsync -r ./libelementrem/test/JsonSpiritHeaders.h     $outputDirectory/test/JsonSpiritHeaders.h
rsync -r ./libelementrem/test/Stats.cpp               $outputDirectory/test/Stats.cpp
rsync -r ./libelementrem/test/Stats.h                 $outputDirectory/test/Stats.h
rsync -r ./libelementrem/test/TestHelper.cpp          $outputDirectory/test/TestHelper.cpp
rsync -r ./libelementrem/test/TestHelper.h            $outputDirectory/test/TestHelper.h
rsync -r ./libelementrem/test/TestUtils.cpp           $outputDirectory/test/TestUtils.cpp
rsync -r ./libelementrem/test/TestUtils.h             $outputDirectory/test/TestUtils.h
rsync -r ./libweb3core/bench/             $outputDirectory/bench/
rsync -r ./libweb3core/libdevcore/        $outputDirectory/libdevcore/
rsync -r ./libweb3core/libdevcrypto/      $outputDirectory/libdevcrypto/
rsync -r ./libweb3core/libp2p/            $outputDirectory/libp2p/
rsync -r ./libweb3core/rlp/               $outputDirectory/rlp/
rsync -r ./libweb3core/test/libdevcore/   $outputDirectory/test/libdevcore/
rsync -r ./libweb3core/test/libdevcrypto/ $outputDirectory/test/libdevcrypto/
rsync -r ./libweb3core/test/libp2p/       $outputDirectory/test/libp2p/
rsync -r ./libweb3core/test/memorydb.cpp  $outputDirectory/test/memorydb.cpp
rsync -r ./libweb3core/test/overlaydb.cpp $outputDirectory/test/overlaydb.cpp
# libweb3core/test/test.cpp and test.h intentionally omitted because they clash with boosttest.cpp from libelementrem/test.
# mix intentionally omitted
# res intentionally omitted
# solidity intentionally omitted
# web3.js intentionally omitted
rsync -r ./webthree/ele/                  $outputDirectory/ele/
rsync -r ./webthree/libweb3jsonrpc/       $outputDirectory/libweb3jsonrpc/
rsync -r ./webthree/libwebthree/          $outputDirectory/libwebthree/
rsync -r ./webthree/libwhisper/           $outputDirectory/libwhisper/
rsync -r ./webthree/test/elerpctest/      $outputDirectory/test/elerpctest/
rsync -r ./webthree/test/libweb3jsonrpc/  $outputDirectory/test/libweb3jsonrpc/
rsync -r ./webthree/test/libwhisper/      $outputDirectory/test/libwhisper/
rsync -r ./webthree-helpers/cmake/        $outputDirectory/cmake/
rsync -r ./webthree-helpers/extdep/       $outputDirectory/extdep/
rsync -r ./webthree-helpers/homebrew/     $outputDirectory/homebrew/
rsync -r ./webthree-helpers/js/           $outputDirectory/js/
rsync -r ./webthree-helpers/scripts/      $outputDirectory/scripts/
rsync -r ./webthree-helpers/templates/    $outputDirectory/templates/
rsync -r ./webthree-helpers/utils/        $outputDirectory/utils/
# intentionally left /webthree-helpers root files behind: (LICENSE, new.sh, README.md)
# TODO /webthree-helpers/cmake has (LICENSE, README.md), but why?
# TODO /webthree-helpers/homebrew has (LICENSE, README.md), but why?
# Tried unsuccessfully to delete homebrew ones.   Needed in some release flow?

# Loose files in the root directory of webthree-umbrella.
# TODO - Move all these loose scripts in the root into /scripts
# CMakeLists.txt intentionally omitted.
rsync -r ./CodingStandards.txt            $outputDirectory/CodingStandards.txt
rsync -r ./CONTRIBUTING.md                $outputDirectory/CONTRIBUTING.md
rsync -r ./GPLV3_LICENSE                  $outputDirectory/GPLV3_LICENSE
rsync -r ./LICENSE                        $outputDirectory/LICENSE
# qtcreator-style intentionally omitted.
# README.md intentionally omitted.
rsync -r ./sanitizer-blacklist.txt        $outputDirectory/sanitizer-blacklist.txt
rsync -r ./sync.sh                        $outputDirectory/sync.sh

# These files cannot be upstreamed, but instead need to be manually maintained and then dropped into 'cpp-elementrem' when we merge.
# These CMakeLists.txt were manually synthesized by Bob.
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/cmake/EleOptions.cmake > $outputDirectory/cmake/EleOptions.cmake
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/CMakeLists.txt > $outputDirectory/CMakeLists.txt
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/README.md > $outputDirectory/README.md
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/test/CMakeLists.txt > $outputDirectory/test/CMakeLists.txt

# These files could be upstreamed, but it isn't worth doing so, because they can only be used after the repo reorganization.
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/.travis.yml > $outputDirectory/.travis.yml
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/appveyor.yml > $outputDirectory/appveyor.yml
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/circle.yml > $outputDirectory/circle.yml
curl https://raw.githubusercontent.com/bobsummerwill/cpp-elementrem/merge_repos/setup.sh > $outputDirectory/setup.sh

# TODO - evmjit submodule will need "hooking up", for now we'll just git clone it into a local directory to get
# the content we need for testing.
git clone https://github.com/elementrem/evmjit $outputDirectory/evmjit

# TODO - Move Contributing and coding standards to http://eledocs.org
# TODO - Where will qtcreator-style go?   Ditto for res folder.
