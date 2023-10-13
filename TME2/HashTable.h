#pragma once
#include <vector>
#include <forward_list>
#include <string>


namespace pr{
    template<typename K, typename V>
    class HashTable{
        public:
        class Entry
        {
            public:
                const K key_;
                V value_;
                Entry(const K& key, const V& value):key_(key), value_(value){}
        };

        private:
        typedef std::vector<std::forward_list<Entry>> bucket_t;
        bucket_t buckets;
        size_t sz;

        public:

        HashTable(size_t init): buckets(init), sz(0){}
        
        V* get(const K& key){
            size_t h = std::hash<K>()(key);
            size_t target = h%buckets.size();
            for(Entry & e:buckets[target]){
                if(e.key_==key)
                    return &e.value_;
            }
            return nullptr;
        }

        size_t size() const{
            return sz;
        }

        bool put(const K & key, const V & value){
            size_t h = std::hash<K>()(key);
            size_t target = h%buckets.size();
            for(Entry & e:buckets[target]){
                if(e.key_==key){
                    e.value_ = value;
                    return true;
                }
            }
            sz++;
            buckets[target].push_front(Entry(key,value));
            return false;
        }

        void grow(){
            if(sz>=(buckets.size()*0.8)){
                HashTable newTable = HashTable(buckets.size()*2);
                for(auto& list:buckets){
                    for(auto& entry:list)
                        newTable.put(entry.key_,entry.value_);
                }
                buckets = std::move(newTable.buckets);
            }
            return;
        }

        class iterator{
            private:
            typename std::vector<std::forward_list<Entry>>& buckets;
            size_t index;
            typename std::forward_list<Entry>::iterator lit;

            public:

            iterator(typename std::vector<std::forward_list<Entry>>& buckets, size_t index, const typename std::forward_list<Entry>::iterator& lit):
            buckets(buckets),index(index),lit(lit){}


            iterator & operator++(){
                ++lit;
                if(lit==buckets[index].end()){
                    index++;
                    while(buckets[index].empty()&&index < buckets.size()){
                        index++;
                    }
                    if(index != buckets.size()-1){
                        lit = buckets[index].begin();
                    }
                    
                }

                return *this;
            }

            bool operator!=(const iterator& other){
                return index != other.index || lit != other.lit;
            }

            Entry & operator*(){
                return *lit;
            }



        };
        
        iterator begin(){
            size_t index = 0;
            while(buckets[index].empty() && index!=sz){
                index++;
            }
            if(index!=sz){
                return iterator(buckets, index, buckets[index].begin());
            }else{
                return end();
            }
        }

        iterator end(){
            return iterator(buckets,buckets.size(),buckets[buckets.size()-1].end());
        }
            
    };

}
