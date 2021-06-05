import 'package:flutter_modular/flutter_modular.dart';
import 'package:http/http.dart' as http;

import 'helpers.dart';
import 'login_service.dart';

import 'env.dart';

class StrikeService {
  static final URL = Uri.parse('$SERVER_URL/striker');

  final loginService = Modular.get<LoginService>();

  Future<bool> tryStrike() async {
    try {
      var res = await http.post(URL, headers: loginService.authHeaders);
      return isHttpSuccess(res.statusCode);
    } catch (e) {
      return false;
    }
  }
}
