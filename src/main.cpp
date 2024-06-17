#include <Geode/Geode.hpp>

using namespace geode::prelude;

void updateVariables(CustomSongWidget* levelInfoWidget, CustomSongWidget* musicLibraryWidget) { //I hate these kinds of work-arounds
	auto downloaded = MusicDownloadManager::sharedState()->isSongDownloaded(musicLibraryWidget->m_customSongID);
	if (levelInfoWidget->m_songs.contains(musicLibraryWidget->m_customSongID)) {
		levelInfoWidget->m_songs.at(musicLibraryWidget->m_customSongID) = downloaded; //Manually editing this grrr
	}
}

void updateButtons(CustomSongWidget* customSongWidget) {
	if (customSongWidget->m_customSongID == 0 && !customSongWidget->m_isRobtopSong && customSongWidget->m_songs.size() == 0) return;
	geode::log::debug("{}", customSongWidget->m_unkBool2);
	auto undownloadedAsset = false;
	if (customSongWidget->m_songs.size() > 0) {
		for (auto song : customSongWidget->m_songs) {
			if (song.second == false) {
				undownloadedAsset = true;
			}
		}
	}
	else if (!MusicDownloadManager::sharedState()->isSongDownloaded(customSongWidget->m_customSongID)) {
		undownloadedAsset = true;
	}
	if (customSongWidget->m_sfx.size() > 0) {
		for (auto sfx : customSongWidget->m_sfx) {
			if (sfx.second == false) {
				undownloadedAsset = true;
			}
		}
	}
	if (undownloadedAsset) {
		geode::log::debug("found redownloadable asset");
		customSongWidget->m_downloadBtn->setVisible(customSongWidget->m_showDownloadBtn);
		customSongWidget->m_deleteBtn->setVisible(false);
	}
	else {
		geode::log::debug("all assets downloaded!");
		customSongWidget->m_deleteBtn->setVisible(!customSongWidget->m_isRobtopSong && customSongWidget->m_showDownloadBtn); //If only robtop song then no delete for now :3
		customSongWidget->m_downloadBtn->setVisible(false);
	}
	if (customSongWidget->m_isMusicLibrary) {
		geode::log::debug("isMusicLibrary");
		customSongWidget->m_selectSongBtn->setPosition(ccp(-227.5f, -160.f));
		customSongWidget->m_playbackBtn->setPosition(ccp(-187.5f, -160.f));
		customSongWidget->m_deleteBtn->setPosition(ccp(-147.5f, -160.f));
		customSongWidget->m_downloadBtn->setPosition(ccp(-147.5f, -160.f));

		customSongWidget->m_songLabel->limitLabelWidth(135.f - (customSongWidget->m_songLabel->getPositionX() + 150), 0.5f, 0.01f);
		customSongWidget->m_songIDLabel->limitLabelWidth(185.0f, 0.6f, 0.01f);
		auto pos = customSongWidget->m_songLabel->getPositionX() + customSongWidget->m_songLabel->getScaledContentWidth() - 280.f;
		customSongWidget->m_artistLabel->limitLabelWidth(-250 - pos, 0.5f, 0.01f);
		customSongWidget->m_moreBtn->setContentSize(ccp(customSongWidget->m_artistLabel->getScaledContentWidth(), 16.f));
		customSongWidget->m_artistLabel->setPosition(customSongWidget->m_moreBtn->getContentSize() / 2);
		customSongWidget->m_moreBtn->setPositionX(pos + (customSongWidget->m_moreBtn->getScaledContentWidth() / 2));

		if (auto levelInfo = static_cast<LevelInfoLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("LevelInfoLayer"))) {
			geode::log::debug("updating levelInfo");
			updateVariables(levelInfo->m_songWidget, customSongWidget);
			updateButtons(levelInfo->m_songWidget); //Should not be recursive because this one isnt musicLibrary widget
		}
	}
}

#include <Geode/modify/CustomSongWidget.hpp>
class $modify(MySongWidget, CustomSongWidget) {
	void deleteSong() {
		CustomSongWidget::deleteSong();
		geode::log::debug("deleteSong");
		updateButtons(this);
	}
	void downloadAssetFinished(int p0, GJAssetType p1) {
		CustomSongWidget::downloadAssetFinished(p0, p1);
		geode::log::debug("downloadAssetFinished");
		updateButtons(this);
	}
	void downloadAssetFailed(int p0, GJAssetType p1, GJSongError p2) {
		CustomSongWidget::downloadAssetFailed(p0, p1, p2);
		geode::log::debug("downloadAssetFailed");
	}
	void downloadFailed() {
		CustomSongWidget::downloadFailed();
		geode::log::debug("downloadFailed");
	}
	void processNextMultiAsset() {
		CustomSongWidget::processNextMultiAsset();
		geode::log::debug("processNextMultiAsset");
	}
	void updateWithMultiAssets(gd::string p0, gd::string p1, int p2) {
		CustomSongWidget::updateWithMultiAssets(p0, p1, p2);
		geode::log::debug("updateWithMultiAssets");
		updateButtons(this);
	}
	void updateMultiAssetInfo(bool p0) {
		CustomSongWidget::updateMultiAssetInfo(p0);
		geode::log::debug("updateMultiAssetInfo");
	}
	void updateDownloadProgress(float p0) {
		CustomSongWidget::updateDownloadProgress(p0);
		geode::log::debug("updateDownloadProgress");
	}
	void updateProgressBar(int p0) {
		CustomSongWidget::updateProgressBar(p0);
		geode::log::debug("updateProgressBar");
	}
	void updateSongInfo() {
		CustomSongWidget::updateSongInfo();
		geode::log::debug("updateSongInfo");
		updateButtons(this);
	}
	void startMultiAssetDownload() {
		CustomSongWidget::startMultiAssetDownload();
		geode::log::debug("startMultiAssetDownload");
	}
	void startDownload() {
		CustomSongWidget::startDownload();
		geode::log::debug("startDownload");
	}
	void downloadSongFinished(int p0) { //For some reason downloadAssetFinished only works with multiple assets (actually kinda makes sense)
		CustomSongWidget::downloadSongFinished(p0);
		geode::log::debug("downloadSongFinished");
	}
	void songStateChanged() {
		CustomSongWidget::songStateChanged();
		geode::log::debug("songStateChanged");
	}
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) return false;

		geode::log::debug(">w< showSongSelect: {}, showPlayMusic: {}, showDownload: {}, isRobtopSong: {}, unkBool: {}, isMusicLibrary: {}, unk: {}", showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk);

		return true;
	}
};