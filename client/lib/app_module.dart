import 'package:flutter/material.dart';
import 'package:flutter_modular/flutter_modular.dart';

import 'app_widget.dart';
import 'login_service.dart';
import 'strike_service.dart';

class AppModule extends MainModule {
  @override
  List<Bind> get binds => [
        Bind((_) => LoginService()),
        Bind((_) => StrikeService()),
      ];

  @override
  List<ModularRouter> get routers => [];

  @override
  Widget get bootstrap => AppWidget();
}
