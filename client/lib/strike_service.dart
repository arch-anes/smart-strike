import 'package:flutter_modular/flutter_modular.dart';
import 'package:http/http.dart' as http;

import 'login_service.dart';

import 'env.dart';

class StrikeService {
  static const URL = '$SERVER_URL/striker';

  final loginService = Modular.get<LoginService>();

  Future<bool> tryStrike() async {
    try {
      await http.post(URL, headers: loginService.authHeaders);
      return true;
    } catch (e) {
      return false;
    }
  }
}
