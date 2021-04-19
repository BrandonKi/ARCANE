#ifndef BYTECODE_GEN_H
#define BYTECODE_GEN_H

#include "AST.h"

// include this to get the instructions
#include "Arcvm.h" 

class BytecodeGen {
    public:
        BytecodeGen(Project*);
        ~BytecodeGen();

        std::vector<u8, arena_allocator<u8>> gen_code();
    
    private:
        Project* ast_;
        std::vector<u8, arena_allocator<u8>> code_;
            
        void gen_project(const Project *project);
        void gen_file(const File*);
        void gen_import(const Import*);
        void gen_function(const Function*);
        void gen_block(const Block*);
        void gen_statement(const Statement*);
        void gen_while(const While_*);
        void gen_for(const For_*);
        void gen_if(const If_*);
        void gen_ret(const Ret*);
        void gen_decl(const Decl*);
        void gen_expr(const Expr*);

        void gen_int_lit(const u64 val);
        void gen_float_lit(const f64 val);
        void gen_string_lit(const astring* val);
        void gen_id(const astring* id);
        void gen_bin(const Expr *expr);
        void gen_unary(const Expr* expr);

        void push(const u8);
        void push(const std::vector<u8, arena_allocator<u8>>&);

        void push_64_bit_value(const u64);
    
        void generate_bootstrap();

        template<
            typename T__,
            typename std::enable_if<
                std::is_integral<T__>::value || std::is_pointer<T__>::value || std::is_floating_point<T__>::value,
                bool
            >::type = true
        >
        constexpr inline void push_value(const T__ val_) {
            
            using T_ =
                typename std::conditional<
                    std::is_same<T__, bool>::value,
                    uint8_t,
                    typename std::conditional<
                        std::is_pointer<T__>::value,
                        uintptr_t,
                        T__
                    >::type
                >::type;
            T_ val;
            if constexpr (std::is_pointer<T__>::value)
                val = reinterpret_cast<T_>(val_);
            else
                val = static_cast<T_>(val_);
            
            using type = 
                typename std::conditional<
                    std::is_signed<T_>::value,
                    typename std::conditional<
                        std::is_floating_point<T_>::value,
                        typename std::conditional<
                            std::is_same<float, T_>::value && sizeof(float) == 4,
                            uint32_t,
                            typename std::conditional<
                                std::is_same<double, T_>::value && sizeof(double) == 8,
                                uint64_t,
                                void
                            >::type
                        >::type,
                    T_
                    >::type,
                T_
                >::type;
            
            static_assert(!std::is_same<void, type>::value, "Only 4 and 8 byte floating point types are supported");
            auto raw_val = std::bit_cast<type>(val);
            // now that we are past all the template stuff we get to the actual code
            // just pushing bytes into a vector
            for (size_t i = 0; i < sizeof(type); ++i)
                code_.push_back(static_cast<unsigned char>((raw_val >> (i * 8)) & 0xff));
        }

        template<typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
        constexpr inline void push_value(const T val) {
            auto new_val = static_cast<typename std::make_unsigned<T>::type>(val);
            push_value(new_val);
        }
};

#endif