#include <cstddef>
#include <format>
#include <print>
#include <string>

using std::format;
using std::println;
using std::string;
using price_t = unsigned int;

// вар 9
//
// Товар, велосипед, горный велосипед, самокат

class Product {
  private:
    price_t price;
    string product_name;
    long article_number;
    string manufacturer;

  public:
    Product(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer
    )
        : price(price), product_name(product_name),
          article_number(article_number), manufacturer(manufacturer) {}

    virtual ~Product() = default;

    price_t get_price() const { return price; }
    string get_product_name() const { return product_name; }
    long get_article_number() const { return article_number; }
    string get_manufacturer() const { return manufacturer; }

    virtual string to_display_string() const;
};

template <>struct std::formatter<Product> {
    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

    auto format(const Product &c, std::format_context ctx) const {
        return std::format_to(
            ctx.out(),
            "Цена: {}\nНаименование: {}\nАртикул: {}\nПроизводитель: {}",
            c.get_price(),
            c.get_product_name(),
            c.get_article_number(),
            c.get_manufacturer()
        );
    }
};

string Product::to_display_string() const{
    return format( "{}", *this);
}

class Bicycle : public Product {
  private:
    unsigned long frame_size;
    size_t gear_count;
    bool suspension;

  public:
    Bicycle(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        size_t frame_size,
        size_t gear_count,
        bool suspension
    )
        : Product(price, product_name, article_number, manufacturer),
          frame_size(frame_size), gear_count(gear_count),
          suspension(suspension) {}

    unsigned long get_frame_size() const { return frame_size; }
    size_t get_gear_count() const { return gear_count; }
    bool has_suspension() const { return suspension; }

    string to_display_string() const override {
        auto hs = "Нет";
        if (suspension) {
            hs = "Да";
        }

        return Product::to_display_string() +
               format(
                   "\nРазмер рамы (мм): {}\nКоличесво передач: {}\nЕсть "
                   "подвеска: {}",
                   frame_size,
                   gear_count,
                   hs
               );
    }
};

class MountainBicycle : public Bicycle {
  private:
    unsigned long tire_width;

  public:
    MountainBicycle(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        size_t frame_size,
        size_t gear_count,
        bool has_suspension,
        unsigned long tire_width
    )
        : Bicycle(
              price,
              product_name,
              article_number,
              manufacturer,
              frame_size,
              gear_count,
              has_suspension
          ),
          tire_width(tire_width) {}

    unsigned long get_tire_width() const { return tire_width; }
    string to_display_string() const override {
        return Bicycle::to_display_string() +
               format("\nШирина покрышки (мм): {}", tire_width);
    }
};

class Scooter : public Product {
  private:
    unsigned long max_load;
    bool foldable;

  public:
    Scooter(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        unsigned long max_load,
        bool foldable
    )
        : Product(price, product_name, article_number, manufacturer),
          max_load(max_load), foldable(foldable) {}

    unsigned long get_max_load() { return max_load; }
    bool is_foldable() { return foldable; }

    string to_display_string() const override {
        auto fld = "Нет";
        if (foldable) {
            fld = "Да";
        }

        return Product::to_display_string() +
               format(
                   "\nМаксимальная загрузка (гр): {}\nСкладной: {}",
                   max_load,
                   fld
               );
    }
};

int main() {
    Bicycle s{15, "aaa", 1500, "shit.co", 1200, 27, true};
    println("{}", s.to_display_string());
};
