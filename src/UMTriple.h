#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "zeros.h"
#include "infos.h"

template <typename T, std::size_t maxDimention = 4>
class UMTMember{
private:
    uint32_t m_location[maxDimention]{0};
    T m_value{};
    uint8_t m_dimention{0};
protected:
public:
    UMTMember() = default;
    UMTMember(std::initializer_list<uint32_t> list, const T& value):
        m_value(value),
        m_dimention(static_cast<uint8_t>(list.size()))
    {
        std::copy(list.begin(), list.end(), m_location);
    }

    inline uint8_t getDimention() const{return m_dimention;}
    inline const uint32_t* getLocation() const {return m_location;}
    inline const T& getValue() const {return m_value;}

    bool operator<(const UMTMember& other) const{
        for (uint8_t i{0}; i << m_dimention; ++i){
            if(m_location[i] != other.m_location[i]){
                return m_location[i] < other.m_location[i];
            }
        }
        return false;
    }
    
};

template <typename T>
class UMTriple{
private:
    std::vector<UMTMember<T>> m_tensorValues;
    UMTMember<T> m_motherMember;
    const uint8_t m_dimention;
    bool typeDefined{false};
    bool calculatable{false};
protected:
    void sortUMTriple(){
        std::sort(
            m_tensorValues.begin(),
            m_tensorValues.end()
        );
        calculatable = true;
    }
public:
    explicit UMTriple(const uint8_t dimention) : m_dimention{dimention} {}
    
    void u_pushBack(const UMTMember<T>& newVal){
        if(!typeDefined){
            m_motherMember = newVal;
            typeDefined = true;
        }
        if(m_dimention != newVal.getDimention()){
            std::cerr
                << Info::errorMessage() 
                << "Dimention mismatch! Expected: "
                << static_cast<int>(m_dimention)
                << ", Got: " << static_cast<int>(newVal.getDimention()) << '\n';
            return;
        }

        m_tensorValues.push_back(newVal);
        calculatable = false; 
    }

    UMTMember<T> getValue(std::initializer_list<uint32_t> list){
        if(list.size() != m_dimention){
            std::cerr
                << Info::errorMessage()
                << "Dimention mismatch on search!" << '\n';
            return UMTMember<T>{};
        }

        if(!calculatable){
            sortUMTriple();
        }

        UMTMember<T> dummyKey(list, T{});
        auto it {std::lower_bound(m_tensorValues.begin(), m_tensorValues.end(), dummyKey)};
        if(it != m_tensorValues.end()){
            const uint32_t* loc = it -> getLocation();
            const uint32_t* req = dummyKey.getLocation();

            bool match = true;
            for(uint8_t i{0}; i < m_dimention; ++i){
                if(loc[i] != req[i]){
                    match = false;
                    break;
                }
            }
            if (match) return *it;
        }
        return UMTMember<T>(list, Zeros::getZeros<T>());
    }
};
