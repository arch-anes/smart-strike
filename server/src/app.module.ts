import { Module } from '@nestjs/common';
import { StrikerModule } from './striker/striker.module';
import { AuthModule } from './auth/auth.module';
import { ServeStaticModule } from '@nestjs/serve-static';
import { join } from 'path';

@Module({
    imports: [
        StrikerModule,
        AuthModule,
        ServeStaticModule.forRoot({
            rootPath: join(__dirname, 'client'),
        }),
    ],
})
export class AppModule {}
