// keeping this around as a reference for now
//TODO remove me in the future

#ifndef BYTECODE_GEN_H
#define BYTECODE_GEN_H

#include "BytecodeLinker.h"

// include this to get the instructions
#include "Arcvm.h" 

using code_block = std::vector<u8, arena_allocator<u8>>;

struct fn_info {
    u64 local_var_index;
    u32 local_var_count;
    const std::vector<Arg, arena_allocator<Arg>>& function_args;
};

class BytecodeGen final {


    public:
    
        explicit BytecodeGen(Project*);
        ~BytecodeGen();

        code_block gen_code();

    private:
    
        Project* ast_;
        code_block code_;

        // use arena allocator for these
        std::unordered_map<std::string, u32> variable_table_;
        std::unordered_map<std::string, code_block> function_table_;
        std::vector<fn_info, arena_allocator<fn_info>> fn_info_stack_;        
        
        code_block gen_project(Project*);
        std::vector<linkable_function, arena_allocator<linkable_function>> gen_file(File*);
        void gen_import(code_block&, const Import*);
        void gen_function(code_block&, const Function*);
        void gen_block(code_block&, const Block*);
        void gen_statement(code_block&, const Statement*);
        void gen_while(code_block&, const While_*);
        void gen_for(code_block&, const For_*);
        void gen_if(code_block&, const If_*);
        void gen_ret(code_block&, const Ret*);
        void gen_decl(code_block&, const Decl*);
        void gen_expr(code_block&, const Expr*);

        void gen_int_lit(code_block&, const i64);
        void gen_float_lit(code_block&, const f64);
        void gen_string_lit(code_block&, const std::string*);
        void gen_id(code_block&, const std::string*);
        void gen_fn_call(code_block&, const Expr *);
        void gen_bin(code_block&, const Expr*);
        void gen_unary(code_block&, const Expr*);

        void push(code_block&, const u8);
        void push_block(code_block&, const code_block&);
        void push_string(code_block&, const std::string&);

        void generate_bootstrap(code_block&);
        bool is_variable(const std::string&);
        bool is_function(const std::string&);
        bool is_function_arg(const std::string&);
        i64 get_function_arg_index(const std::string&);

        void deallocate_local_vars(code_block&, u64);

        template<
            typename T__,
            typename std::enable_if<
                std::is_integral<T__>::value || std::is_pointer<T__>::value || std::is_floating_point<T__>::value,
                bool
            >::type = true
        >
        constexpr inline void push_value(code_block& code, const T__ val_) {
            
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
                code.push_back(static_cast<unsigned char>((raw_val >> (i * 8)) & 0xff));
        }

        template<typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type>
        constexpr inline void push_value(code_block& code, const T val) {
            auto new_val = static_cast<typename std::make_unsigned<T>::type>(val);
            push_value(code, new_val);
        }
};

#endif