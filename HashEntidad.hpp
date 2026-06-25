#ifndef __HASHENTIDAD_HPP__
#define __HASHENTIDAD_HPP__

template <typename K, typename V>
class HashEntidad {
private:
	K key;
	V value;
public:
	HashEntidad() {
		this->key = K();   
		this->value = V();
	}

	HashEntidad(K key, V value) {
		this->key = key;
		this->value = value;
	}

	K getKey() { return key; }
	V getValue() { return value; }
};

#endif // !__HASHENTIDAD_HPP__

