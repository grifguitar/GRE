#include "./src/include/hindex.hpp"
#include "../indexInterface.h"

template <class KEY_TYPE, class PAYLOAD_TYPE>
class h2indexInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
   public:
    void init(Param* param = nullptr) {}

    void bulk_load(std::pair<KEY_TYPE, PAYLOAD_TYPE>* key_value, size_t num,
                   Param* param = nullptr);

    bool get(KEY_TYPE key, PAYLOAD_TYPE& val, Param* param = nullptr);

    bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param* param = nullptr);

    bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param* param = nullptr);

    bool remove(KEY_TYPE key, Param* param = nullptr);

    size_t scan(KEY_TYPE key_low_bound, size_t key_num,
                std::pair<KEY_TYPE, PAYLOAD_TYPE>* result,
                Param* param = nullptr);

    long long memory_consumption() { return 0; }

    hindex<KEY_TYPE, PAYLOAD_TYPE>* index;
};

template <class KEY_TYPE, class PAYLOAD_TYPE>
void h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::bulk_load(
    std::pair<KEY_TYPE, PAYLOAD_TYPE>* key_value, size_t num, Param* param) {
    std::vector<KEY_TYPE> key_temp;
    std::vector<PAYLOAD_TYPE*> val_temp;
    key_temp.reserve(num);
    val_temp.reserve(num);
    for (size_t i = 0; i < num; i++) {
        key_temp.push_back(key_value[i].first);
        val_temp.push_back(new PAYLOAD_TYPE(key_value[i].second));
    }
    index = hindex<KEY_TYPE, PAYLOAD_TYPE>::make(key_temp, val_temp, 32);
}

template <class KEY_TYPE, class PAYLOAD_TYPE>
bool h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key,
                                                   PAYLOAD_TYPE& val,
                                                   Param* param) {
    PAYLOAD_TYPE* out = nullptr;
    int res = index->find(key, out);
    if (res == verdict::OK) {
        val = *out;
        return true;
    }
    return false;
}

template <class KEY_TYPE, class PAYLOAD_TYPE>
bool h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key,
                                                   PAYLOAD_TYPE value,
                                                   Param* param) {
    PAYLOAD_TYPE* heap_value = new PAYLOAD_TYPE(value);
    int res = index->insert(key, heap_value);
    if (res == verdict::OK) {
        return true;
    }
    return false;
}

template <class KEY_TYPE, class PAYLOAD_TYPE>
bool h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::update(KEY_TYPE key,
                                                      PAYLOAD_TYPE value,
                                                      Param* param) {
    int res = index->remove(key);
    if (res == verdict::OK) {
        PAYLOAD_TYPE* heap_value = new PAYLOAD_TYPE(value);
        res = index->insert(key, heap_value);
        if (res == verdict::OK) {
            return true;
        }
        return false;
    }
    return false;
}

template <class KEY_TYPE, class PAYLOAD_TYPE>
bool h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::remove(KEY_TYPE key,
                                                      Param* param) {
    int res = index->remove(key);
    if (res == verdict::OK) {
        return true;
    }
    return false;
}

template <class KEY_TYPE, class PAYLOAD_TYPE>
size_t h2indexInterface<KEY_TYPE, PAYLOAD_TYPE>::scan(
    KEY_TYPE key_low_bound, size_t key_num,
    std::pair<KEY_TYPE, PAYLOAD_TYPE>* result, Param* param) {
    throw std::runtime_error("unsupported");
}
