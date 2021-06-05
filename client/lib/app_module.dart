import 'package:flutter/material.dart';
import 'package:flutter_modular/flutter_modular.dart';

import 'app_widget.dart';
import 'login_service.dart';
import 'strike_service.dart';

class AppModule extends Module {
  @override
  final List<Bind> binds = [
    Bind.singleton((_) => LoginService()),
    Bind.singleton((_) => StrikeService()),
  ];

  @override
  final List<ModularRoute> routes = [];
}
