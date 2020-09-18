import 'package:http/http.dart' as http;

class Striker {
  static final url = 'http://localhost:3000/striker';

  static void strike() {
    http.post(url);
  }
}
