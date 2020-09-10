import { Controller, Post, Body, Res, HttpStatus, Get, UseGuards } from '@nestjs/common';
import { AuthService } from './auth.service';
import { AuthPayload } from './auth.interfaces';
import { JwtAuthGuard } from './jwt-auth.guard';

@Controller('oauth2')
export class OAuth2Controller {
    @Get('authorize')
    authorize() {}

    @Post('decision')
    authorize() {}

    @Post('token')
    authorize() {}
}
