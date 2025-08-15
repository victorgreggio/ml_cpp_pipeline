#pragma once

// generated for sqlpp23 - updated from sqlpp11

#include <sqlpp23/sqlpp23.h>

namespace iris
{
  namespace Iris_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct SepalL
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "sepal_l";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T sepalL;
            T& operator()() { return sepalL; }
            const T& operator()() const { return sepalL; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::floating_point, sqlpp::tag::can_be_null>;
    };
    struct SepalW
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "sepal_w";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T sepalW;
            T& operator()() { return sepalW; }
            const T& operator()() const { return sepalW; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::floating_point, sqlpp::tag::can_be_null>;
    };
    struct PetalL
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "petal_l";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T petalL;
            T& operator()() { return petalL; }
            const T& operator()() const { return petalL; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::floating_point, sqlpp::tag::can_be_null>;
    };
    struct PetalW
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "petal_w";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T petalW;
            T& operator()() { return petalW; }
            const T& operator()() const { return petalW; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::floating_point, sqlpp::tag::can_be_null>;
    };
    struct Specie
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "specie";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T specie;
            T& operator()() { return specie; }
            const T& operator()() const { return specie; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace Iris_

  struct Iris: sqlpp::table_t<Iris,
               Iris_::Id,
               Iris_::SepalL,
               Iris_::SepalW,
               Iris_::PetalL,
               Iris_::PetalW,
               Iris_::Specie>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "iris";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T iris;
        T& operator()() { return iris; }
        const T& operator()() const { return iris; }
      };
    };
  };
} // namespace iris
