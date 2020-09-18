import { Controller, Post, Body, Res, HttpStatus, Get, UseGuards } from '@nestjs/common';
import { AuthService } from './auth.service';
import { AuthPayload } from './auth.interfaces';
import { JwtAuthGuard } from './jwt-auth.guard';

@Controller('auth')
export class AuthController {
    constructor(private authService: AuthService) {}

    @Post()
    login(@Body() payload: AuthPayload, @Res() res) {
        try {
            return res.send(this.authService.login(payload));
        } catch (error) {
            return res.status(HttpStatus.UNAUTHORIZED).send(error.toString());
        }
    }

    @UseGuards(JwtAuthGuard)
    @Get()
    refreshToken() {
        return this.authService.generateToken();
    }
}
