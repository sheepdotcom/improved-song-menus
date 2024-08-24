#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CustomSongWidget.hpp>
class $modify(MySongWidget, CustomSongWidget) {
	void updateButtons() {
		if (this->m_customSongID == 0 && !this->m_isRobtopSong && this->m_songs.size() == 0) return;
		//geode::log::debug("{}", this->m_showSelectSongBtn);
		auto undownloadedAsset = false;
		if (this->m_songs.size() > 0) {
			for (auto song : this->m_songs) {
				if (song.second == false) {
					undownloadedAsset = true;
				}
			}
		}
		else if (!MusicDownloadManager::sharedState()->isSongDownloaded(this->m_customSongID) && !this->m_isRobtopSong) {
			undownloadedAsset = true;
		}
		if (this->m_sfx.size() > 0) {
			for (auto sfx : this->m_sfx) {
				if (sfx.second == false) {
					undownloadedAsset = true;
				}
			}
		}
		if (undownloadedAsset) {
			//geode::log::debug("found redownloadable asset");
			this->m_downloadBtn->setVisible(this->m_showDownloadBtn);
			this->m_deleteBtn->setVisible(false);
		}
		else {
			//geode::log::debug("all assets downloaded!");
			if (!this->m_showSelectSongBtn) {
				this->m_deleteBtn->setVisible(!this->m_isRobtopSong && this->m_showDownloadBtn); //If only robtop song then no delete for now :3
				this->m_downloadBtn->setVisible(false);
			}
		}
		if (this->m_isMusicLibrary) {
			//geode::log::debug("isMusicLibrary");
			this->m_selectSongBtn->setPosition(ccp(-227.5f, -160.f));
			this->m_playbackBtn->setPosition(ccp(-187.5f, -160.f));
			this->m_deleteBtn->setPosition(ccp(-147.5f, -160.f));
			this->m_downloadBtn->setPosition(ccp(-147.5f, -160.f));

			if (this->m_showSelectSongBtn) {
				this->m_songLabel->limitLabelWidth((235.f - (this->m_songLabel->getPositionX() + 150)) / 2, 0.5f, 0.01f);
				this->m_songIDLabel->limitLabelWidth(185.0f, 0.6f, 0.01f);
				auto pos = this->m_songLabel->getPositionX() + this->m_songLabel->getScaledContentWidth() - 280.f;
				this->m_artistLabel->limitLabelWidth(-250 - pos, 0.5f, 0.01f);
				this->m_moreBtn->setContentSize(ccp(this->m_artistLabel->getScaledContentWidth(), 16.f));
				this->m_artistLabel->setPosition(this->m_moreBtn->getContentSize() / 2);
				this->m_moreBtn->setPositionX(pos + (this->m_moreBtn->getScaledContentWidth() / 2));
			}
			//Update the one in levelinfo too
			if (auto levelInfo = static_cast<LevelInfoLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("LevelInfoLayer"))) {
				//geode::log::debug("updating levelInfo");
				auto levelInfoWidget = static_cast<MySongWidget*>(levelInfo->m_songWidget);
				auto downloaded = MusicDownloadManager::sharedState()->isSongDownloaded(this->m_customSongID);
				if (levelInfoWidget->m_songs.contains(this->m_customSongID)) {
					levelInfoWidget->m_songs.at(this->m_customSongID) = downloaded; //Manually editing this grrr
				}
				levelInfoWidget->updateButtons(); //Should not be recursive because this one isnt musicLibrary widget
			}
		}
	}

	void deleteSong() {
		CustomSongWidget::deleteSong();
		//geode::log::debug("deleteSong");
		this->updateButtons();
	}
	void downloadAssetFinished(int p0, GJAssetType p1) {
		CustomSongWidget::downloadAssetFinished(p0, p1);
		//geode::log::debug("downloadAssetFinished");
		this->updateButtons();
	}
	void downloadAssetFailed(int p0, GJAssetType p1, GJSongError p2) {
		CustomSongWidget::downloadAssetFailed(p0, p1, p2);
		//geode::log::debug("downloadAssetFailed");
	}
	void downloadFailed() {
		CustomSongWidget::downloadFailed();
		//geode::log::debug("downloadFailed");
	}
	void processNextMultiAsset() {
		CustomSongWidget::processNextMultiAsset();
		//geode::log::debug("processNextMultiAsset");
	}
	void updateWithMultiAssets(gd::string p0, gd::string p1, int p2) {
		CustomSongWidget::updateWithMultiAssets(p0, p1, p2);
		//geode::log::debug("updateWithMultiAssets");
		this->updateButtons();
	}
	void updateMultiAssetInfo(bool p0) {
		CustomSongWidget::updateMultiAssetInfo(p0);
		//geode::log::debug("updateMultiAssetInfo");
	}
	void updateDownloadProgress(float p0) {
		CustomSongWidget::updateDownloadProgress(p0);
		//geode::log::debug("updateDownloadProgress");
	}
	void updateProgressBar(int p0) {
		CustomSongWidget::updateProgressBar(p0);
		//geode::log::debug("updateProgressBar");
	}
	void updateSongInfo() {
		CustomSongWidget::updateSongInfo();
		//geode::log::debug("updateSongInfo");
		this->updateButtons();
	}
	void startMultiAssetDownload() {
		CustomSongWidget::startMultiAssetDownload();
		//geode::log::debug("startMultiAssetDownload");
	}
	void startDownload() {
		CustomSongWidget::startDownload();
		//geode::log::debug("startDownload");
	}
	void downloadSongFinished(int p0) { //For some reason downloadAssetFinished only works with multiple assets (actually kinda makes sense)
		CustomSongWidget::downloadSongFinished(p0);
		//geode::log::debug("downloadSongFinished");
	}
	void songStateChanged() {
		CustomSongWidget::songStateChanged();
		//geode::log::debug("songStateChanged");
	}
	void loadSongInfoFinished(SongInfoObject* p0) {
		CustomSongWidget::loadSongInfoFinished(p0);
		//geode::log::debug("loadSongInfoFinished {}", p0->m_fileSize);
	}
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) return false;

		//geode::log::debug(">w< showSongSelect: {}, showPlayMusic: {}, showDownload: {}, isRobtopSong: {}, unkBool: {}, isMusicLibrary: {}, unk: {}", showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk);

		return true;
	}
};