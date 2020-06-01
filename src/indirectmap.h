// Copyright (c) 2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_INDIRECTMAP_H
#define BITCOIN_INDIRECTMAP_H

template <class T>
struct DereferencingComparator { bool operator()( T a,  T b)  { return *a < *b; } };

/* Map whose keys are pointers, but are compared by their dereferenced values.
 *
 * Differs from a plain std::map< K*, T, DereferencingComparator<K*> > in
 * that methods that take a key for comparison take a K rather than taking a K*
 * (taking a K* would be confusing, since it's the value rather than the address
 * of the object for comparison that matters due to the dereferencing comparator).
 *
 * Objects pointed to by keys must not be modified in any way that changes the
 * result of DereferencingComparator.
 */
template <class K, class T>
class indirectmap {
private:
    typedef std::map< K*, T, DereferencingComparator< K*> > base;
    base m;
public:
    typedef typename base::iterator iterator;
    typedef typename base::const_iterator const_iterator;
    typedef typename base::size_type size_type;
    typedef typename base::value_type value_type;

    // passthrough (pointer interface)
    std::pair<iterator, bool> insert( value_type& value) { return m.insert(value); }

    // pass address (value interface)
    iterator find( K& key)                     { return m.find(&key); }
    const_iterator find( K& key)          { return m.find(&key); }
    iterator lower_bound( K& key)              { return m.lower_bound(&key); }
    const_iterator lower_bound( K& key)   { return m.lower_bound(&key); }
    size_type erase( K& key)                   { return m.erase(&key); }
    size_type count( K& key)              { return m.count(&key); }

    // passthrough
    bool empty()               { return m.empty(); }
    size_type size()           { return m.size(); }
    size_type max_size()       { return m.max_size(); }
    void clear()                    { m.clear(); }
    iterator begin()                { return m.begin(); }
    iterator end()                  { return m.end(); }
    const_iterator begin()     { return m.begin(); }
    const_iterator end()       { return m.end(); }
    const_iterator cbegin()    { return m.cbegin(); }
    const_iterator cend()      { return m.cend(); }
};

#endif // BITCOIN_INDIRECTMAP_H
