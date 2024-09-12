#ifndef BASE_FUNCTION_RESOLVER_H_
#define BASE_FUNCTION_RESOLVER_H_
namespace base {
class FunctionResolver {
 public:
  explicit FunctionResolver(uint64_t base_addr) : base_addr_(base_addr) {}

  template <typename FuncType>
  FuncType ResolveFunction(uint64_t offset) {
    return reinterpret_cast<FuncType>(base_addr_ + offset);
  }

 private:
  uint64_t base_addr_;
};

template<typename FuncType>
auto CastFunction(FunctionResolver& resolver, uint64_t offset) {
    return resolver.ResolveFunction<FuncType>(offset);
}

}  // namespace base
#endif