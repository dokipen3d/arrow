#include "PlugTypes.h"
#include <memory>

class object_t {
public:
  template <typename T> object_t(T x) : self_(make_unique<model<T>>(move(x))) {}

  object_t(const object_t &x) : self_(x.self_->copy_()) {}
  object_t(object_t &&) noexcept = default;
  object_t &operator=(const object_t &x) { return *this = object_t(x); }
  object_t &operator=(object_t &&) noexcept = default;

  void viewClicked(keyStoreStruct key, int senderID) {}
  void viewDragged(keyStoreStruct key, int senderID) {}
  void viewReleased(keyStoreStruct key, int senderID) {}

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual std::unique_ptr<concept_t> copy_() const = 0;
    virtual void viewClicked(keyStoreStruct key, int senderID) = 0;
    virtual void viewDragged(keyStoreStruct key, int senderID) = 0;
    virtual void viewReleased(keyStoreStruct key, int senderID) = 0;
    virtual void resolveSize() = 0;

  };

  template <typename T> struct model final : concept_t {
    model(T x) : data_(move(x)) {}
    unique_ptr<concept_t> copy_() const override {
      return make_unique<model>(*this);
    }

    void viewClicked(keyStoreStruct key, int senderID) override {
      data_.viewClicked(key, senderID);
    }
    void viewDragged(keyStoreStruct key, int senderID) override {
      data_.viewDragged(key, senderID);
    }
    void viewReleased(keyStoreStruct key, int senderID) override {
      data_.viewReleased(key, senderID);
    }
    virtual void resolveSize() override {
      data_.resolveSize();
    }


    T data_;
  };

  std::unique_ptr<concept_t> self_;
};