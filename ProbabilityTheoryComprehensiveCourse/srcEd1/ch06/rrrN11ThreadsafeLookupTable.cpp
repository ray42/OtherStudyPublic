/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<utility> // pair
#include<list> // list
#include<shared_mutex> // shared_mutex
#include<thread> // thread
#include<algorithm> // find_if
#include<vector>//vector
#include<memory> // unique_ptr

template<typename Key, typename Value, typename Hash=std::hash<Key> >
class threadsafe_lookup_table
{
private:
  class bucket_type
  {
  private:
    using bucket_value = std::pair<Key,Value>;
    using bucket_data = std::list<bucket_value>;
    using bucket_iterator = typename bucket_data::iterator;

    bucket_data data;
    mutable std::shared_mutex mutex;                            //1

    // Returns iterator into the BUCKET, pointing to the first bucket_value
    // such that the keys match.
    bucket_iterator find_entry_for(Key const& key) const        //2
    {
      return std::find_if(data.begin(),data.end(),
          [&](bucket_value const& item)
          {
            return item.first == key;
          });
    }
  public:

    // Default value is something the user provides, which is returned if
    // the key does not exist
    Value value_for(Key const& key, Value const& default_value) const
    {
      std::shared_lock<std::shared_mutex> lock(mutex);          //3
      bucket_iterator const found_entry = find_entry_for(key);
      return (found_entry==data.end())?
             default_value : found_entry->second;
    }

    void add_or_update_mapping(Key const& key, Value const& value)
    {
      std::unique_lock<std::shared_mutex> lock(mutex);          //4
      bucket_iterator const found_entry = find_entry_for(key);
      if(found_entry == data.end())
      {
        data.push_back(bucket_value(key,value));
      }
      else
      {
        found_entry->second = value;
      }
    }

    void remove_mapping(Key const& key)
    {
      std::unique_lock<std::shared_mutex> lock(mutex);          //5
      bucket_iterator const found_entry = find_entry_for(key);
      if(found_entry != data.end())
      {
        data.erase(found_entry);
      }
    }
  };

  // End of bucket_type class
  ///////////////////////////////////

  // This is the hash map
  std::vector<std::unique_ptr<bucket_type> > buckets;           //6
  // This is the hashing function: std::hash<Key>
  Hash hasher;

  bucket_type& get_bucket(Key const& key) const                 //7
  {
    std::size_t const bucket_index = hasher(key)%buckets.size();
    return *buckets[bucket_index];
  }

public:
  using key_type = Key;
  using mapped_type = Value;
  using hash_type = Hash;

  // default constructor
  threadsafe_lookup_table(unsigned num_buckets=19,
                          Hash const& hasher_=Hash{}):
    buckets(num_buckets), hasher(hasher_)
  {
    // Loop through the numbers of buckets
    for(unsigned i=0; i < num_buckets; ++i)
    {
      // set the unique pointers
      buckets[i].reset(std::make_unique<bucket_type>());
    }
  }

  // Delete copy constructor and copy assignment operator
  threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
  threadsafe_lookup_table& operator=(
      threadsafe_lookup_table const& other)=delete;

  Value value_for(Key const& key,
                  Value const& default_value=Value{}) const
  {
    return get_bucket(key).value_for(key,default_value);      //8
  }

  void add_or_update_mapping(Key const& key, Value const& value)
  {
    get_bucket(key).add_or_update_mapping(key,value);         //9
  }

  void remove_mapping(Key const& key)
  {
    get_bucket(key).remove_mapping(key);                      //10
  }
};


int main()
{
  return 0;
}

















