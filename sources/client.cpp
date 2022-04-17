// Copyright 2022 Petrova Kseniya <ksyushki5@yandex.ru>
#include "client.hpp"

Client::Client(){}

void Client::request()
{
  std::cout << "WELCOME TO SUGGEST SERVER!" << std::endl;
  std::cout << "Input your request in format  "
               "{\"input\":\"<request>\"}";
  std::cout << std::endl;
  std::cout << "Your input: ";
  std::string inp;
  std::cin >> inp;
  _req = inp;
}

void Client::start()
{
  auto const host = "127.0.0.1";
  auto const port = "80";
  auto const target = "/v1/api/suggest";
  // int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;
  int version = 11; //версия http 11
  //IO-контекст (отвечает за ввод-вывод)
  net::io_context ioc;

  //Объекты ввода-вывода
  net::ip::tcp::resolver resolver(ioc);
  beast::tcp_stream stream(ioc);

  //Поиск доменного имени
  auto const results = resolver.resolve(host, port);
  //Установка соединения по IP
  stream.connect(results);

  // Настройка HTTP-запроса на получение сообщения
  http::request<http::string_body> req{http::verb::post,
                                       target, version};
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  //Тело запроса
  req.body() = _req;
  //Подготовка сообщения для тела
  req.prepare_payload();
  // Отправка запроса
  http::write(stream, req);
  //Буфер для чтения
  beast::flat_buffer buffer;
  //Объявление контейнера для хранения ответа
  http::response<http::dynamic_body> res;
  //Получение ответа
  http::read(stream, buffer, res);
  //Вывод сообщения на экран
  std::cout << res << std::endl;

  //Закрываем сокет
  beast::error_code ec;
  stream.socket().shutdown(net::ip::tcp::socket::shutdown_both, ec);

  //Обработка ошибки отсутствия соединения
  if (ec && ec != beast::errc::not_connected)
    throw beast::system_error{ec};
}

Client::~Client(){}
