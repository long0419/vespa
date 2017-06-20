// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#pragma once

namespace search::btree {

class BtreeNoLeafData;

namespace test {

template <typename ostream, typename DataT>
void printData(ostream &os, const DataT &data);

template <typename ostream, typename DataT>
void printData(ostream &os, const DataT &data)
{
    os << ":" << data;
}

template <typename ostream>
void printData(ostream &os, const BTreeNoLeafData &data)
{
    (void) os;
    (void) data;
}

} // namespace search::btree::test
} // namespace search::btree
