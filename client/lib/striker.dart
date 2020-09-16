import 'package:http/http.dart' as http;

class Striker {
  static final url = 'http://door-strike/strike';

  static void strike() {
    http.get(url);
  }
}
