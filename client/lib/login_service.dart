import 'package:flutter/foundation.dart';
import 'package:http/http.dart' as http;
import 'dart:convert' as convert;
import 'package:localstorage/localstorage.dart';

import 'env.dart';

class LoginService {
  static const TOKEN_KEY = "token";
  static final URL = Uri.parse('$SERVER_URL/auth');

  final localStorage = LocalStorage('login');
  final isLoggedIn = ValueNotifier(false);

  String __token;

  LoginService() {
    init();
  }

  init() async {
    await localStorage.ready;
    _token = localStorage.getItem(TOKEN_KEY);
    if (token != null) await _refreshToken();
  }

  get token {
    return __token;
  }

  set _token(value) {
    __token = value;
    isLoggedIn.value = value != null;
    localStorage.setItem(TOKEN_KEY, value);
  }

  Map<String, String> get authHeaders {
    return {"Authorization": "Bearer $token"};
  }

  tryLogin(String password) async {
    try {
      var response = await http.post(URL, body: {'password': password});
      var jsonResponse = convert.jsonDecode(response.body);
      _token = jsonResponse['token'];
      return true;
    } catch (e) {
      disconnect();
      return false;
    }
  }

  disconnect() {
    _token = null;
  }

  _refreshToken() async {
    try {
      var response = await http.get(URL, headers: authHeaders);
      var jsonResponse = convert.jsonDecode(response.body);
      _token = jsonResponse['token'];
    } catch (e) {
      disconnect();
    }
  }
}
