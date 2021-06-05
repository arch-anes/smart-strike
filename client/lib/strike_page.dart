import 'package:flutter/material.dart';
import 'package:flutter_modular/flutter_modular.dart';

import 'strike_service.dart';
import 'timeout_button.dart';

class StrikePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final strikeService = Modular.get<StrikeService>();

    return Material(
      color: Colors.white,
      child: Center(child: TimeoutButton(callback: strikeService.tryStrike)),
    );
  }
}
