#include <ostream>

template< class CharT, class Traits> std::basic_ostream<CharT,Traits>& operator<<( std::basic_ostream<CharT,Traits>& os,CharT ch ){
    
}

template< class CharT, class Traits> std::basic_ostream<CharT,Traits>& operator<<( std::basic_ostream<CharT,Traits>& os,char ch ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,char ch ){

}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,signed char ch ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,unsigned char ch ){
    
}

template< class CharT, class Traits >std::basic_ostream<CharT,Traits>& operator<<( std::basic_ostream<CharT,Traits>& os,
                                         const CharT* s ){
    
}

template< class CharT, class Traits >std::basic_ostream<CharT,Traits>& operator<<( std::basic_ostream<CharT,Traits>& os,
                                         const char* s ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,  
                                        const char* s ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,
                                        const signed char* s ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,
                                        const unsigned char* s ){
    
}

template< class Ostream, class T >Ostream&&operator<<( Ostream&& os,const T& value ){
    
}

template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,wchar_t ch ) = delete;
template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,char16_t ch ) = delete;
template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,char32_t ch ) = delete;
template< class Traits >std::basic_ostream<wchar_t,Traits>& operator<<( std::basic_ostream<wchar_t,Traits>& os,char16_t ch ) = delete;
template< class Traits >std::basic_ostream<wchar_t,Traits>& operator<<( std::basic_ostream<wchar_t,Traits>& os,char32_t ch ) = delete;
template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,const wchar_t* ch ) = delete;
template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,const char16_t* ch ) = delete;
template< class Traits >std::basic_ostream<char,Traits>& operator<<( std::basic_ostream<char,Traits>& os,const char32_t* ch ) = delete;
template< class Traits >std::basic_ostream<wchar_t,Traits>& operator<<( std::basic_ostream<wchar_t,Traits>& os,const char16_t* ch ) = delete;
template< class Traits >std::basic_ostream<wchar_t,Traits>& operator<<( std::basic_ostream<wchar_t,Traits>& os,const char32_t* ch ) = delete;
