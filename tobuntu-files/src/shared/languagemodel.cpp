/*
 * Copyright (C) 2014 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "languagemodel.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>

LanguageModel::LanguageModel(QObject *parent) :
    SelectionModel(parent)
{
    QMap<QString, QString> languages;
    languages.insert(tr("Afar"), "aa");
    languages.insert(tr("Abkhazian"), "ab");
    languages.insert(tr("Afrikaans"), "af");
    languages.insert(tr("Akan"), "af");
    languages.insert(tr("Albanian"), "sq");
    languages.insert(tr("Amharic"), "am");
    languages.insert(tr("Arabic"), "ar");
    languages.insert(tr("Aragonese"), "an");
    languages.insert(tr("Armenian"), "hy");
    languages.insert(tr("Assamese"), "as");
    languages.insert(tr("Avaric"), "av");
    languages.insert(tr("Avestan"), "ae");
    languages.insert(tr("Aymara"), "ay");
    languages.insert(tr("Azerbaijani"), "az");
    languages.insert(tr("Bashkir"), "ba");
    languages.insert(tr("Bambara"), "bm");
    languages.insert(tr("Basque"), "eu");
    languages.insert(tr("Belarusian"), "be");
    languages.insert(tr("Bengali"), "bn");
    languages.insert(tr("Bihari languages"), "bh");
    languages.insert(tr("Bislama"), "bi");
    languages.insert(tr("Bosnian"), "bs");
    languages.insert(tr("Breton"), "br");
    languages.insert(tr("Bulgarian"), "bg");
    languages.insert(tr("Burmese"), "my");
    languages.insert(tr("Catalan"), "ca");
    languages.insert(tr("Central Khmer"), "km");
    languages.insert(tr("Chamorro"), "ch");
    languages.insert(tr("Chechen"), "ce");
    languages.insert(tr("Chichewa"), "ny");
    languages.insert(tr("Chinese"), "zh");
    languages.insert(tr("Church Slavic"), "cu");
    languages.insert(tr("Chuvash"), "cv");
    languages.insert(tr("Cornish"), "kw");
    languages.insert(tr("Corsican"), "co");
    languages.insert(tr("Cree"), "cr");
    languages.insert(tr("Croatian"), "hr");
    languages.insert(tr("Czech"), "cs");
    languages.insert(tr("Danish"), "da");
    languages.insert(tr("Divehi"), "dv");
    languages.insert(tr("Dutch"), "nl");
    languages.insert(tr("Dzongkha"), "dz");
    languages.insert(tr("English"), "en");
    languages.insert(tr("Esperanto"), "eo");
    languages.insert(tr("Estonian"), "et");
    languages.insert(tr("Ewe"), "ee");
    languages.insert(tr("Faroese"), "fo");
    languages.insert(tr("Fijian"), "fj");
    languages.insert(tr("Finnish"), "fi");
    languages.insert(tr("French"), "fr");
    languages.insert(tr("Fulah"), "ff");
    languages.insert(tr("Gaelic"), "gd");
    languages.insert(tr("Galician"), "gl");
    languages.insert(tr("Ganda"), "lg");
    languages.insert(tr("Georgian"), "ka");
    languages.insert(tr("German"), "de");
    languages.insert(tr("Greek"), "el");
    languages.insert(tr("Guarani"), "gn");
    languages.insert(tr("Gujarati"), "gu");
    languages.insert(tr("Haitian"), "ht");
    languages.insert(tr("Hausa"), "ha");
    languages.insert(tr("Hebrew"), "he");
    languages.insert(tr("Herero"), "hz");
    languages.insert(tr("Hindi"), "hi");
    languages.insert(tr("Hiri Motu"), "ho");
    languages.insert(tr("Hungarian"), "hu");
    languages.insert(tr("Icelandic"), "is");
    languages.insert(tr("Ido"), "io");
    languages.insert(tr("Igbo"), "ig");
    languages.insert(tr("Indonesian"), "id");
    languages.insert(tr("Inuktitut"), "iu");
    languages.insert(tr("Interlingua"), "ia");
    languages.insert(tr("Interlingue"), "ie");
    languages.insert(tr("Inupiaq"), "ik");
    languages.insert(tr("Irish"), "ga");
    languages.insert(tr("Italian"), "it");
    languages.insert(tr("Japanese"), "ja");
    languages.insert(tr("Javanese"), "jv");
    languages.insert(tr("Kalaallisut"), "kl");
    languages.insert(tr("Kannada"), "kn");
    languages.insert(tr("Kanuri"), "kr");
    languages.insert(tr("Kashmiri"), "ks");
    languages.insert(tr("Kazakh"), "kk");
    languages.insert(tr("Kikuyu"), "ki");
    languages.insert(tr("Kinyarwanda"), "rw");
    languages.insert(tr("Kirghiz"), "ky");
    languages.insert(tr("Komi"), "kv");
    languages.insert(tr("Kongo"), "kg");
    languages.insert(tr("Korean"), "ko");
    languages.insert(tr("Kuanyama"), "kj");
    languages.insert(tr("Kurdish"), "ku");
    languages.insert(tr("Lao"), "lo");
    languages.insert(tr("Latin"), "la");
    languages.insert(tr("Latvian"), "lv");
    languages.insert(tr("Limburgan"), "li");
    languages.insert(tr("Lingala"), "ln");
    languages.insert(tr("Lithuanian"), "lt");
    languages.insert(tr("Luxembourgish"), "lb");
    languages.insert(tr("Luba-Katanga"), "lu");
    languages.insert(tr("Macedonian"), "mk");
    languages.insert(tr("Malagasy"), "mg");
    languages.insert(tr("Malay"), "ms");
    languages.insert(tr("Malayalam"), "ml");
    languages.insert(tr("Maltese"), "mt");
    languages.insert(tr("Manx"), "gv");
    languages.insert(tr("Maori"), "mi");
    languages.insert(tr("Marathi"), "mr");
    languages.insert(tr("Marshallese"), "mh");
    languages.insert(tr("Mongolian"), "mn");
    languages.insert(tr("Nauru"), "na");
    languages.insert(tr("Navajo"), "nv");
    languages.insert(tr("Ndebele, North"), "nd");
    languages.insert(tr("Ndebele, South"), "nr");
    languages.insert(tr("Ndonga"), "ng");
    languages.insert(tr("Nepali"), "ne");
    languages.insert(tr("Northern Sami"), "se");
    languages.insert(tr("Norwegian"), "no");
    languages.insert(tr("Norwegian Bokmål"), "nb");
    languages.insert(tr("Norwegian Nynorsk"), "nn");
    languages.insert(tr("Occitan"), "oc");
    languages.insert(tr("Ojibwa"), "oj");
    languages.insert(tr("Oriya"), "or");
    languages.insert(tr("Oromo"), "om");
    languages.insert(tr("Ossetian"), "os");
    languages.insert(tr("Pali"), "pi");
    languages.insert(tr("Persian"), "fa");
    languages.insert(tr("Polish"), "pl");
    languages.insert(tr("Portuguese"), "pt");
    languages.insert(tr("Punjabi"), "pa");
    languages.insert(tr("Pushto"), "ps");
    languages.insert(tr("Quechua"), "qu");
    languages.insert(tr("Romansh"), "rm");
    languages.insert(tr("Romanian"), "ro");
    languages.insert(tr("Rundi"), "rn");
    languages.insert(tr("Russian"), "ru");
    languages.insert(tr("Samoan"), "sm");
    languages.insert(tr("Sango"), "sg");
    languages.insert(tr("Sanskrit"), "sa");
    languages.insert(tr("Sardinian"), "sc");
    languages.insert(tr("Serbian"), "sr");
    languages.insert(tr("Shona"), "sn");
    languages.insert(tr("Sindhi"), "sd");
    languages.insert(tr("Sinhala"), "si");
    languages.insert(tr("Sichuan Yi"), "ii");
    languages.insert(tr("Slovak"), "sk");
    languages.insert(tr("Slovenian"), "sl");
    languages.insert(tr("Somali"), "so");
    languages.insert(tr("Sotho, Southern"), "st");
    languages.insert(tr("Spanish"), "es");
    languages.insert(tr("Sundanese"), "su");
    languages.insert(tr("Swahili"), "sw");
    languages.insert(tr("Swati"), "ss");
    languages.insert(tr("Swedish"), "sv");
    languages.insert(tr("Tagalog"), "tl");
    languages.insert(tr("Tahitian"), "ty");
    languages.insert(tr("Tajik"), "tg");
    languages.insert(tr("Tamil"), "ta");
    languages.insert(tr("Tatar"), "tt");
    languages.insert(tr("Telugu"), "te");
    languages.insert(tr("Thai"), "th");
    languages.insert(tr("Tibetan"), "bo");
    languages.insert(tr("Tigrinya"), "ti");
    languages.insert(tr("Tonga"), "to");
    languages.insert(tr("Tsonga"), "ts");
    languages.insert(tr("Tswana"), "tn");
    languages.insert(tr("Turkish"), "tr");
    languages.insert(tr("Turkmen"), "tk");
    languages.insert(tr("Twi"), "tw");
    languages.insert(tr("Uighur"), "ug");
    languages.insert(tr("Ukrainian"), "uk");
    languages.insert(tr("Urdu"), "ur");
    languages.insert(tr("Uzbek"), "uz");
    languages.insert(tr("Venda"), "ve");
    languages.insert(tr("Vietnamese"), "vi");
    languages.insert(tr("Volapük"), "vo");
    languages.insert(tr("Walloon"), "wa");
    languages.insert(tr("Welsh"), "cy");
    languages.insert(tr("Western Frisian"), "fy");
    languages.insert(tr("Wolof"), "wo");
    languages.insert(tr("Xhosa"), "xh");
    languages.insert(tr("Yiddish"), "yi");
    languages.insert(tr("Yoruba"), "yo");
    languages.insert(tr("Zhuang"), "za");
    languages.insert(tr("Zulu"), "zu");

    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("translations");

    foreach (QString translation, dir.entryList(QStringList() << "*.qm", QDir::Files, QDir::Name)) {
        QString code = translation.left(2);
        this->addItem(languages.key(code), code);
    }
}

LanguageModel::~LanguageModel() {}
